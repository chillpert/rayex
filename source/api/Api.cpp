#include "api/Api.hpp"
#include "api/misc/Components.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Destructors.hpp"

namespace RENDERER_NAMESPACE
{
  const std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };
#ifdef RX_DEBUG
  std::vector<const char*> extensions = { "VK_KHR_get_physical_device_properties2", "VK_EXT_debug_utils" };
#else
  std::vector<const char*> extensions = { "VK_KHR_get_physical_device_properties2" };
#endif

  std::vector<const char*> deviceExtensions =
  {
    VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
    VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
    VK_KHR_MAINTENANCE3_EXTENSION_NAME,
    VK_KHR_RAY_TRACING_EXTENSION_NAME
  };

  size_t currentFrame = 0;

  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  Api::Api( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera ) :
    window( window ),
    camera( camera ),
    gui( nullptr )
  { }

  Api::Api( std::shared_ptr<Window> window, std::shared_ptr<Gui> gui, std::shared_ptr<Camera> camera ) :
    window( window ),
    camera( camera ),
    gui( gui )
  { }

  Api::~Api( )
  {
    g_device.waitIdle( );

    // Gui needs to be destroyed manually, as RAII destruction will not be possible.
    if ( this->gui != nullptr )
      this->gui->destroy( );
  }

  void Api::setGui( const std::shared_ptr<Gui> gui, bool initialize )
  {
    if ( this->gui != nullptr )
    {
      recreateSwapchain( );
      this->gui->destroy( );
    }

    this->gui = gui;

    if ( initialize )
      initGui( );
  }

  bool Api::init( )
  {
    if ( this->settings == nullptr )
    {
      RX_ERROR( "No rendering settings provided. Closing application." );
      return false;
    }

    RX_INFO( "Initializing Vulkan API ..." );

    this->geometryNodes.reserve( g_maxGeometryNodes );
    this->textures.reserve( g_maxTextures );
    this->dirLightNodes.reserve( g_maxLightNodes );
    this->pointLightNodes.reserve( g_maxLightNodes );

    bool result = true;

    // Instance
    result = this->instance.init( layers, extensions );
    
    // Debug messenger.
    result = this->debugMessenger.init( vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );

    // Surface
    result = this->surface.init( );
    
    // Physical device
    auto temp = vk::Initializer::initPhysicalDevice( );
    this->physicalDevice = temp.first;
    result = temp.second;

    // Reassess the support of the preferred surface settings.
    this->surface.checkSettingSupport( );

    // Queues
    Queues queues;
    queues.init( );

    // Logical device
    this->device.init( deviceExtensions );

    // Retrieve all queue handles.
    queues.retrieveHandles( );

    // Render pass
    initRenderPass( );

    // Swapchain
    this->swapchain.init( &this->surface, this->renderPass.get( ) );

    // Command pools
    this->graphicsCmdPool = vk::Initializer::initCommandPoolUnique( g_graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
    g_graphicsCmdPool = this->graphicsCmdPool.get( );

    this->transferCmdPool = vk::Initializer::initCommandPoolUnique( g_transferFamilyIndex, { } );
    g_transferCmdPool = this->transferCmdPool.get( );

    // Descriptor sets and layouts
    initDescriptorSets( );

    // SSBO for scene description
    this->rtInstances.push_back( { } );
    this->rayTracingInstancesBuffer.init<RayTracingInstance>( this->rtInstances );
    this->rtInstances.clear( );

    this->sceneDescriptorSets.update( this->lightsUniformBuffer.getRaw( ), this->rayTracingInstancesBuffer.get( ) );

    // Ray tracing pipeline
    this->rtPipeline.init( { this->rtDescriptorSetLayout.get( ), this->modelDescriptorSetLayout.get( ), this->sceneDescriptorSetLayout.get( ) }, this->settings );

    // Rasterization pipeline
    glm::vec2 extent = { static_cast<float>( this->swapchain.getExtent( ).width ), static_cast<float>( this->swapchain.getExtent( ).height ) };
    this->viewport = vk::Viewport( 0.0f, 0.0f, extent.x, extent.y, 0.0f, 1.0f );
    this->scissor = vk::Rect2D( 0, this->swapchain.getExtent( ) );
    this->rsPipeline.init( { this->rsDescriptorSetLayout.get( ) }, this->renderPass.get( ), viewport, scissor );
    
    // Ray tracing
    this->rayTracingBuilder.init( );
    this->rayTracingBuilder.createStorageImage( this->swapchain.getExtent( ) );
    this->rayTracingBuilder.createShaderBindingTable( this->rtPipeline.get( ) );
    
    // Swapchain command buffers
    this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );

    // GUI
    initGui( );

    // Record swapchain command buffers.
    recordSwapchainCommandBuffers( );

    // Create fences and semaphores.
    initSyncObjects( );

    // Make sure swapchain images are presentable in case they were not transitioned automatically.
    this->swapchain.setImageLayout( vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR );

    return result;
  }

  void Api::update( )
  {
    if ( this->settings->refresh )
    {
      this->settings->refresh = false;

      // Trigger swapchain / pipeline recreation
      RX_WARN( "Settings were changed. Swapchain recreation necessary." );

      recreateSwapchain( );
    }

    uint32_t imageIndex = this->swapchain.getCurrentImageIndex( );
    
    if ( this->settings->rayTrace )
    {
      // Upload camera
      if ( this->camera != nullptr )
      {
        if ( this->camera->updateView )
        {
          this->cameraUbo.view = this->camera->getViewMatrix( );
          this->cameraUbo.viewInverse = this->camera->getViewInverseMatrix( );
      
          this->camera->updateView = false;
        }

        if ( this->camera->updateProj )
        {
          this->cameraUbo.projection = this->camera->getProjectionMatrix( );
          this->cameraUbo.projectionInverse = this->camera->getProjectionInverseMatrix( );
    
          this->camera->updateProj = false;
        }
      }
      this->cameraUniformBuffer.upload<CameraUbo>( imageIndex, this->cameraUbo );

      // Upload lights
      LightsUbo lightNodeUbos;

      for ( size_t i = 0; i < this->dirLightNodes.size( ); ++i )
        lightNodeUbos.directionalLightNodes[i] = this->dirLightNodes[i]->toUbo( );

      for ( size_t i = 0; i < this->pointLightNodes.size( ); ++i )
        lightNodeUbos.pointLightNodes[i] = this->pointLightNodes[i]->toUbo( );

      this->lightsUniformBuffer.upload<LightsUbo>( imageIndex, lightNodeUbos );

      // Upload scene description
      if ( this->uploadSceneDescriptionData )
      {
        this->uploadSceneDescriptionData = false;
        this->rayTracingInstancesBuffer.fill<RayTracingInstance>( this->rtInstances.data( ) );
      }
    }
    else
    {
      for ( std::shared_ptr<GeometryNode> node : this->geometryNodes )
      {
        if ( node->modelPath.empty( ) )
          continue;

        RasterizationUbo ubo
        {
          node->worldTransform,
          camera->getViewMatrix( ),
          camera->getProjectionMatrix( ),
          camera->getPosition( )
        };

        node->rsUniformBuffer.upload( imageIndex, ubo );
      }
    }
  }

  bool Api::prepareFrame( )
  {
    // Wait for the current frame's fences.
    vk::Result result = g_device.waitForFences( 1, &this->inFlightFences[currentFrame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

    // If the window is minimized then simply do not render anything anymore.
    if ( this->window->minimized( ) )
      return true;

    // If the window size has changed the swapchain has to be recreated.
    if ( this->window->changed( ) || this->needSwapchainRecreate )
    {
      this->camera->updateProj = true;

      this->needSwapchainRecreate = false;
      recreateSwapchain( );
      return true;
    }

    this->swapchain.acquireNextImage( this->imageAvailableSemaphores[currentFrame].get( ), nullptr );

    return false;
  }

  bool Api::submitFrame( )
  {
    uint32_t imageIndex = this->swapchain.getCurrentImageIndex( );
  
    // Check if a previous frame is using the current image.
    if ( this->imagesInFlight[imageIndex] )
    {
      vk::Result result = g_device.waitForFences( 1, &this->imagesInFlight[currentFrame], VK_TRUE, UINT64_MAX );
      RX_ASSERT( ( result == vk::Result::eSuccess ), "Failed to wait for fences." );
    }

    // This will mark the current image to be in use by this frame.
    this->imagesInFlight[imageIndex] = this->inFlightFences[currentFrame].get( );

    // Add GUI command buffer to swapchain command buffer if GUI is enabled.
    std::vector<vk::CommandBuffer> commandBuffers = { this->swapchainCommandBuffers.get( )[imageIndex] };
    if ( this->gui != nullptr )
      commandBuffers.push_back( this->gui->getCommandBuffer( imageIndex ) );

    // Reset the signaled state of the current frame's fence to the unsignaled one.
    g_device.resetFences( 1, &this->inFlightFences[currentFrame].get( ) );

    // Submits / executes the current image's / framebuffer's command buffer.
    auto pWaitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    auto submitInfo = vk::Helper::getSubmitInfo( this->imageAvailableSemaphores[currentFrame].get( ), this->finishedRenderSemaphores[currentFrame].get( ), commandBuffers, pWaitDstStageMask );
    g_graphicsQueue.submit( submitInfo, this->inFlightFences[currentFrame].get( ) );

    // Tell the presentation engine that the current image is ready.
    auto presentInfo = vk::Helper::getPresentInfoKHR( this->finishedRenderSemaphores[currentFrame].get( ), imageIndex );

    try
    {
      vk::Result result = g_graphicsQueue.presentKHR( presentInfo );
      if ( result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR )
      {
        this->needSwapchainRecreate = true;
        RX_WARN( "Swapchain out of data or suboptimal." );
      }
    }
    catch ( ... )
    {
      this->needSwapchainRecreate = true;
    }

    currentFrame = ( currentFrame + 1 ) % maxFramesInFlight;
    return false;
  }

  bool Api::render( )
  {
    if ( this->gui != nullptr )
    {
      this->gui->newFrame( );
      this->gui->render( );
      this->gui->endRender( );
    }

    if ( prepareFrame( ) )
      return true;

    if ( this->gui != nullptr )
      this->gui->renderDrawData( this->swapchain.getCurrentImageIndex( ) );

    if ( submitFrame( ) )
      return true;

    return true;
  }

  void Api::recreateSwapchain( )
  {
    RX_INFO( "Recreating swapchain." );

    g_device.waitIdle( );

    // Clean up existing swapchain and dependencies.
    this->swapchainCommandBuffers.free( );
    this->swapchain.destroy( );

    // Recreating the swapchain.
    this->swapchain.init( &this->surface, this->renderPass.get( ) );

    // Recreate storage image with the new swapchain image size and update the ray tracing descriptor set to use the new storage image view.
    this->rayTracingBuilder.createStorageImage( this->swapchain.getExtent( ) );
    this->rtDescriptorSets.update( this->rayTracingBuilder.getTlas( ).as.as, 
                                   this->rayTracingBuilder.getStorageImageView( ), 
                                   this->cameraUniformBuffer.getRaw( ) );

    // Swapchain command buffers
    this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
    recordSwapchainCommandBuffers( );

    if ( this->gui != nullptr )
      this->gui->recreate( this->swapchain.getExtent( ), this->swapchain.getImageViews( ) );

    // Update the camera screen size to avoid image stretching.
    auto screenSize = this->swapchain.getExtent( );
    this->camera->setSize( screenSize.width, screenSize.height );
    
    RX_SUCCESS( "Finished swapchain re-creation." );
  }

  void Api::initRenderPass( )
  {
    auto colorAttachmentDescription = vk::Helper::getAttachmentDescription( this->surface.getFormat( ) );

    vk::AttachmentReference colorAttachmentReference( 0,                                          // attachment
                                                      vk::ImageLayout::eColorAttachmentOptimal ); // layout

    auto depthAttachmentDescription = vk::Helper::getDepthAttachmentDescription( getSupportedDepthFormat( g_physicalDevice ) );

    vk::AttachmentReference depthAttachmentRef( 1,                                                 // attachment
                                                vk::ImageLayout::eDepthStencilAttachmentOptimal ); // layout

    vk::SubpassDescription subpassDescription( { },                              // flags
                                               vk::PipelineBindPoint::eGraphics, // pipelineBindPoint
                                               0,                                // inputAttachmentsCount
                                               nullptr,                          // pInputAttachments
                                               1,                                // colorAttachmentsCount
                                               &colorAttachmentReference,        // pColorAttachments
                                               nullptr,                          // pResolveAttachments
                                               &depthAttachmentRef,              // pDepthStencilAttachment
                                               0,                                // preserveAttachemntCount
                                               nullptr );                        // pPreserveAttachments

    std::vector<vk::SubpassDependency> subpassDependencies( 2 );

    subpassDependencies[0] = { VK_SUBPASS_EXTERNAL,                                                                  // srcSubpass
                               0,                                                                                    // dstSubpass
                               vk::PipelineStageFlagBits::eBottomOfPipe,                                             // srcStageMask
                               vk::PipelineStageFlagBits::eColorAttachmentOutput,                                    // dstStageMask
                               vk::AccessFlagBits::eMemoryRead,                                                      // srcAccessMask
                               vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite, // dstAccessMask
                               vk::DependencyFlagBits::eByRegion };                                                  // dependencyFlags

    subpassDependencies[1] = { 0,                                                                                    // srcSubpass
                               VK_SUBPASS_EXTERNAL,                                                                  // dstSubpass
                               vk::PipelineStageFlagBits::eColorAttachmentOutput,                                    // srcStageMask
                               vk::PipelineStageFlagBits::eBottomOfPipe,                                             // dstStageMask
                               vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite, // srcAccessMask
                               vk::AccessFlagBits::eMemoryRead,                                                      // dstAccessMask
                               vk::DependencyFlagBits::eByRegion };                                                  // dependencyFlags

    this->renderPass.init( { colorAttachmentDescription, depthAttachmentDescription }, { subpassDescription }, subpassDependencies );
  }

  /// @todo add handling for missing textures, models etc.
  void Api::initModel( const std::shared_ptr<GeometryNode> node )
  {
    auto it = this->models.find( node->modelPath );
    auto model = it->second;
    if ( !model->initialized )
    {
      model->vertexBuffer.init( model->vertices );
      model->indexBuffer.init( model->indices );

      model->rtDescriptorSets.init( this->modelDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->modelDescriptorSetLayout.get( ) } );
      model->rsDescriptorSets.init( this->rsDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rsDescriptorSetLayout.get( ) } );
  
      model->initialized = true;
    }

    node->rtInstance.modelIndex = model->index;
    // TODO: Try to call this as few times as possible.   
    this->rayTracingBuilder.createBottomLevelAS( vk::Helper::unpack( this->models ) );
    this->rayTracingBuilder.createTopLevelAS( this->geometryNodes );

    // Update ray tracing descriptor set.
    this->rtDescriptorSets.update( this->rayTracingBuilder.getTlas( ).as.as, this->rayTracingBuilder.getStorageImageView( ), this->cameraUniformBuffer.getRaw( ) );

    // Rasterization uniform buffer
    node->rsUniformBuffer.init<RasterizationUbo>( static_cast<uint32_t>( g_swapchainImageCount ) );

    // Update second descriptor set.
    auto diffuseIter = this->textures.find( node->material.diffuseTexture[0] );
    model->rtDescriptorSets.update( diffuseIter->second->getImageView( ), diffuseIter->second->getSampler( ), model->vertexBuffer.get( ), model->indexBuffer.get( ) );
    model->rsDescriptorSets.update( node->rsUniformBuffer.getRaw( ), diffuseIter->second->getImageView( ), diffuseIter->second->getSampler( ) );
  }

  void Api::initGui( )
  {
    if ( this->gui != nullptr )
    {
      initRenderPass( );
      this->gui->init( &this->surface, this->swapchain.getExtent( ), this->swapchain.getImageViews( ) );
    }
  }

  void Api::recordSwapchainCommandBuffers( )
  {
    RX_INFO( "Started swapchain command buffers recording." );

    if ( this->settings->rayTrace )
    {
      // Start recording the swapchain framebuffers
      for ( size_t imageIndex = 0; imageIndex < this->swapchainCommandBuffers.get( ).size( ); ++imageIndex )
      {
        this->swapchainCommandBuffers.begin( imageIndex );

        this->swapchainCommandBuffers.get( imageIndex ).pushConstants( this->rtPipeline.getLayout( ),       // layout
                                                                       vk::ShaderStageFlagBits::eMissKHR,   // stageFlags
                                                                       0,                                   // offset
                                                                       sizeof( glm::vec4 ),                 // size
                                                                       &this->settings->getClearColor( ) ); // pValues

        this->swapchainCommandBuffers.get( imageIndex ).pushConstants( this->rtPipeline.getLayout( ),           // layout
                                                                       vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                                                       sizeof( glm::vec4 ),                     // offset
                                                                       sizeof( glm::vec4 ),                     // size
                                                                       &this->settings->getClearColor( ) );     // pValues

        this->swapchainCommandBuffers.get( imageIndex ).bindPipeline( vk::PipelineBindPoint::eRayTracingKHR, this->rtPipeline.get( ) );

        for ( const auto& node : this->geometryNodes )
        {
          auto iter = this->models.find( node->modelPath );
          RX_ASSERT( ( iter->second != nullptr ), "Can not find model" );

          std::vector<vk::DescriptorSet> descriptorSets = { this->rtDescriptorSets.get( imageIndex ), iter->second->rtDescriptorSets.get( imageIndex ), this->sceneDescriptorSets.get( imageIndex ) };

          this->swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eRayTracingKHR,
                                                                              this->rtPipeline.getLayout( ),
                                                                              0,                                                           // first set
                                                                              static_cast<uint32_t>( descriptorSets.size( ) ),             // descriptor set count
                                                                              descriptorSets.data( ),                                      // descriptor sets
                                                                              0,                                                           // dynamic offset count
                                                                              nullptr );                                                   // dynamic offsets 

          this->rayTracingBuilder.rayTrace( this->swapchainCommandBuffers.get( imageIndex ), this->swapchain.getImage( imageIndex ), this->swapchain.getExtent( ) );
        }

        this->swapchainCommandBuffers.end( imageIndex );
      }
    }
    else
    {
      // Set up render pass begin info
      std::array<vk::ClearValue, 2> clearValues;
      clearValues[0].color = { util::vec4toArray( this->settings->getClearColor( ) ) };
      clearValues[1].depthStencil = vk::ClearDepthStencilValue { 1.0f, 0 };

      // Start recording the swapchain framebuffers
      for ( size_t imageIndex = 0; imageIndex < this->swapchainCommandBuffers.get( ).size( ); ++imageIndex )
      {
        this->swapchainCommandBuffers.begin( imageIndex );

        this->renderPass.begin( this->swapchain.getFramebuffer(imageIndex),
                                this->swapchainCommandBuffers.get( imageIndex ),
                                { 0, this->swapchain.getExtent( ) },
                                { clearValues[0], clearValues[1] } );

        this->swapchainCommandBuffers.get( imageIndex ).bindPipeline( vk::PipelineBindPoint::eGraphics, this->rsPipeline.get( ) ); // CMD

        // Dynamic states
        vk::Viewport viewport = this->viewport;
        viewport.width = this->window->getWidth( );
        viewport.height = this->window->getHeight( );

        this->swapchainCommandBuffers.get( imageIndex ).setViewport( 0, 1, &viewport ); // CMD

        vk::Rect2D scissor = this->scissor;
        scissor.extent = this->window->getExtent( );

        this->swapchainCommandBuffers.get( imageIndex ).setScissor( 0, 1, &scissor ); // CMD

        // Draw models
        for ( const auto& node : this->geometryNodes )
        {
          if ( node->modelPath.empty( ) )
            continue;

          auto it = this->models.find( node->modelPath );
          RX_ASSERT( ( it != this->models.end( ) ), "Can not draw model because it was not found." );

          auto model = it->second;

          vk::Buffer vertexBuffers[] = { model->vertexBuffer.get( ) };
          vk::DeviceSize offsets[] = { 0 };

          this->swapchainCommandBuffers.get( imageIndex ).bindVertexBuffers( 0,               // first binding
                                                                             1,               // binding count
                                                                             vertexBuffers,
                                                                             offsets );


          this->swapchainCommandBuffers.get( imageIndex ).bindIndexBuffer( model->indexBuffer.get( ),
                                                                           0,                                 // offset
                                                                           model->indexBuffer.getType( ) );

          // TODO: Also bind ray tracing descriptor set.
          this->swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eGraphics,
                                                                              rsPipeline.getLayout( ),
                                                                              0,                                         // first set
                                                                              1,                                         // descriptor set count
                                                                              &model->rsDescriptorSets.get( )[imageIndex], // descriptor sets
                                                                              0,                                         // dynamic offset count
                                                                              nullptr );                                 // dynamic offsets 

          this->swapchainCommandBuffers.get( imageIndex ).drawIndexed( model->indexBuffer.getCount( ), // index count
                                                                       1,                              // instance count
                                                                       0,                              // first index
                                                                       0,                              // vertex offset
                                                                       0 );                            // first instance

        }

        this->renderPass.end( this->swapchainCommandBuffers.get( imageIndex ) );
        this->swapchainCommandBuffers.end( imageIndex );
      }
    }
  }

  void Api::initSyncObjects( )
  {
    this->imageAvailableSemaphores.resize( maxFramesInFlight );
    this->finishedRenderSemaphores.resize( maxFramesInFlight );
    this->inFlightFences.resize( maxFramesInFlight );
    this->imagesInFlight.resize( g_swapchainImageCount, nullptr );

    for ( size_t i = 0; i < maxFramesInFlight; ++i )
    {
      this->imageAvailableSemaphores[i] = vk::Initializer::initSemaphoreUnique( );
      this->finishedRenderSemaphores[i] = vk::Initializer::initSemaphoreUnique( );
      this->inFlightFences[i] = vk::Initializer::initFenceUnique( );
    }
  }

  void Api::initDescriptorSets( )
  {
    // Create the ray tracing descriptor set layout
    {
      // TLAS
      auto tlasBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eAccelerationStructureKHR, vk::ShaderStageFlagBits::eRaygenKHR );
      // Output image
      auto outputImageBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eRaygenKHR );
      // Camera uniform buffer
      auto camUboBinding = vk::Helper::getDescriptorSetLayoutBinding( 2, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eRaygenKHR );

      this->rtDescriptorSetLayout.init( { tlasBinding, outputImageBinding, camUboBinding } );
    }

    // Create the descriptor set layout for models
    {
      // Texture image
      auto textureBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Vertex buffer
      auto vertexBufferBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Index buffer                                 
      auto indexBufferBinding = vk::Helper::getDescriptorSetLayoutBinding( 2, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      this->modelDescriptorSetLayout.init( { textureBinding, vertexBufferBinding, indexBufferBinding } );
    }
    
    // Scene descriptor set layout.
    {
      // Directional light nodes uniform buffer
      auto dirLightsUboBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Scene description buffer
      auto sceneDescriptionBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      this->sceneDescriptorSetLayout.init( { dirLightsUboBinding, sceneDescriptionBinding } );
    }

    // Rasterization descriptor set layout.
    {
      // Uniform buffer binding for the vertex shader.
      auto uboBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex );
      // Texture image
      auto textureBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment );

      this->rsDescriptorSetLayout.init( { uboBinding, textureBinding } );
    }

    // Ray tracing descriptor pool
    this->rtDescriptorPool = vk::Initializer::initDescriptorPoolUnique( vk::Helper::getPoolSizes( this->rtDescriptorSetLayout.getBindings( ) ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    // Model descriptor pool
    this->modelDescriptorPool = vk::Initializer::initDescriptorPoolUnique( vk::Helper::getPoolSizes( this->modelDescriptorSetLayout.getBindings( ) ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    // Scene descriptor pool
    this->sceneDescriptorPool = vk::Initializer::initDescriptorPoolUnique( vk::Helper::getPoolSizes( this->sceneDescriptorSetLayout.getBindings( ) ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    // Rasterization descriptor pool
    this->rsDescriptorPool = vk::Initializer::initDescriptorPoolUnique( vk::Helper::getPoolSizes( this->rsDescriptorSetLayout.getBindings( ) ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    // Ray tracing descriptor sets
    this->rtDescriptorSets.init( this->rtDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rtDescriptorSetLayout.get( ) } );

    // Rasterization descriptor set
    this->rsDescriptorSets.init( this->rsDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rtDescriptorSetLayout.get( ) } );

    // Scene descriptor sets
    this->sceneDescriptorSets.init( this->sceneDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->sceneDescriptorSetLayout.get( ) } );

    // Uniform buffers for camera
    this->cameraUniformBuffer.init<CameraUbo>( static_cast<uint32_t>( g_swapchainImageCount ) );

    // Uniform buffers for light nodes
    this->lightsUniformBuffer.init<LightsUbo>( static_cast<uint32_t>( g_swapchainImageCount ) );
  }
}
