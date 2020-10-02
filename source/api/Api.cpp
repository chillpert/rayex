#include "api/Api.hpp"
#include "api/misc/Components.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Destructors.hpp"
#include "api/utility/Util.hpp"

namespace RAYEXEC_NAMESPACE
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
    VK_KHR_RAY_TRACING_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

  void Api::init( )
  {
    this->textures.reserve( g_maxTextures );

    // Instance
    instance = vk::Initializer::initInstance( layers, extensions );

    // Debug messenger.
    this->debugMessenger.init( );

    // Surface
    this->surface.init( );

    // Physical device
    this->physicalDevice = vk::Initializer::initPhysicalDevice(  );

    // Reassess the support of the preferred surface settings.
    this->surface.assessSettings( );

    // Queues
    vk::Initializer::initQueueFamilyIndices( );

    // Logical device
    device = vk::Initializer::initDevice( deviceExtensions );

    // Retrieve all queue handles.
    g_device.getQueue( g_graphicsFamilyIndex, 0, &g_graphicsQueue );
    g_device.getQueue( g_transferFamilyIndex, 0, &g_transferQueue );

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

    // Set up buffers for the scene description data (light sources, rtInstances, camera).
    initSceneStorageBuffer( );

    // Initialize a rasterization and raytracing pipeline.
    initPipelines( );

    // Ray tracing
    this->rayTracingBuilder.init( );
    this->rayTracingBuilder.createStorageImage( this->swapchain.getExtent( ) );
    this->rayTracingBuilder.createShaderBindingTable( this->rtPipeline.get( ) );

    this->settings->maxRecursionDepth = this->rayTracingBuilder.getRtProperties( ).maxRecursionDepth;

    // Swapchain command buffers
    this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );

    // GUI
    initGui( );

    // Record swapchain command buffers.
    recordSwapchainCommandBuffers( );

    // Create fences and semaphores.
    initSyncObjects( );
  }

  void Api::update( )
  {
    updateSettings( );

    updateUniformBuffers( );

    // Upload scene description
    if ( this->uploadSceneDescriptionData )
    {
      this->uploadSceneDescriptionData = false;
      this->rayTracingInstancesBuffer.fill<RayTracingInstance>( this->rtInstances.data( ) );
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
    RX_INFO( "Recreating swapchain ..." );

    g_device.waitIdle( );

    // Clean up existing swapchain and dependencies.
    this->swapchainCommandBuffers.free( );
    this->swapchain.destroy( );

    // Recreating the swapchain.
    this->swapchain.init( &this->surface, this->renderPass.get( ) );

    // Recreate storage image with the new swapchain image size and update the ray tracing descriptor set to use the new storage image view.
    this->rayTracingBuilder.createStorageImage( this->swapchain.getExtent( ) );

    AccelerationStructureDescriptor tlas { this->rayTracingBuilder.getTlas( ).as.as };
    StorageImageDescriptor storageImage { this->rayTracingBuilder.getStorageImageView( ) };
    UboDescriptor cameraUbo { this->cameraUniformBuffer.getRaw( ), sizeof( CameraUbo ) };

    this->rtDescriptorSets.update( { tlas, storageImage, cameraUbo } );

    // Swapchain command buffers
    this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
    recordSwapchainCommandBuffers( );

    if ( this->gui != nullptr )
      this->gui->recreate( this->swapchain.getExtent( ), this->swapchain.getImageViews( ) );

    // Update the camera screen size to avoid image stretching.
    auto screenSize = this->swapchain.getExtent( );
    this->camera->setSize( screenSize.width, screenSize.height );

    RX_SUCCESS( "Swapchain recreation finished." );
  }

  void Api::popNode( const std::shared_ptr<Node> node )
  {
    if ( node == nullptr )
    {
      RX_ERROR( "Can not pop node because node is nullptr." );
      return;
    }

    if ( dynamic_cast<GeometryNode*>( node.get( ) ) )
    {
      auto ptr = std::dynamic_pointer_cast<GeometryNode>( node );
      
      // Delete node
      for ( const auto& it : this->geometryNodes )
      {
        if ( it == ptr )
        {

        }
      }
    }
    else if ( dynamic_cast<LightNode*>( node.get( ) ) )
    {

    }
    else
    {
      RX_ERROR( "Failed to pop node because type can not be handled." );
    }
  }

  void Api::updateAccelerationStructure( )
  {
    // TODO: Try to call this as few times as possible.
    this->rayTracingBuilder.createBottomLevelAS( vk::Helper::unpack( this->models ) );
    this->rayTracingBuilder.createTopLevelAS( this->geometryNodes );

    // Update ray tracing descriptor set.
    AccelerationStructureDescriptor tlas { this->rayTracingBuilder.getTlas( ).as.as };
    StorageImageDescriptor storageImage { this->rayTracingBuilder.getStorageImageView( ) };
    UboDescriptor camera { this->cameraUniformBuffer.getRaw( ), sizeof( CameraUbo ) };

    this->rtDescriptorSets.update( { tlas, storageImage, camera } );
  }

  void Api::initPipelines( )
  {
    // Ray tracing pipeline
    std::vector<vk::DescriptorSetLayout> temp = { this->rtDescriptorSetLayout.get( ), this->rtModelDescriptorSetLayout.get( ), this->rtSceneDescriptorSetLayout.get( ) };
    if ( this->vertexDataSetLayout.get( ) )
    {
      temp.push_back( this->vertexDataSetLayout.get( ) );
      RX_WARN( "Adding model data set layout." );
    }

    this->rtPipeline.init( temp, this->settings );

    // Rasterization pipeline
    glm::vec2 extent = { static_cast<float>( this->swapchain.getExtent( ).width ), static_cast<float>( this->swapchain.getExtent( ).height ) };
    this->viewport = vk::Viewport( 0.0f, 0.0f, extent.x, extent.y, 0.0f, 1.0f );
    this->scissor = vk::Rect2D( 0, this->swapchain.getExtent( ) );
    return this->rsPipeline.init( { this->rsSceneDescriptorSetLayout.get( ), this->rsModelDescriptorSetLayout.get( ) }, this->renderPass.get( ), viewport, scissor, this->settings );
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
      RX_FATAL( "THIS SHOULD NOT HAPPEN" );

      model->vertexBuffer.init( model->vertices );
      model->indexBuffer.init( model->indices );

      //this->allVertices.insert( this->allVertices.begin( ), model->vertices.begin( ), model->vertices.end( ) );
      //this->allIndices.insert( this->allIndices.begin( ), model->indices.begin( ), model->indices.end( ) );
      //this->allVertices = model->vertices;
      //this->allIndices = model->indices;

      static size_t vertexCount = 0;
      vertexCount += model->vertices.size( );
      static size_t indexCount = 0;
      indexCount += model->indices.size( );
      
      RX_WARN( vertexCount, " | ", indexCount );

      model->rtDescriptorSets.init( this->rtModelDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rtModelDescriptorSetLayout.get( ) } );
      model->rsDescriptorSets.init( this->rsModelDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rsModelDescriptorSetLayout.get( ) } );
      
      model->initialized = true;
    }

    std::cout << model->index << ", " << model->path << std::endl;

    updateAccelerationStructure( );
    
    // The model's rasterization descriptors.
    StorageBufferDescriptor vertexBuffer { model->vertexBuffer.get( ) };
    StorageBufferDescriptor indexBuffer  { model->indexBuffer.get( ) };

    model->rtDescriptorSets.update( { vertexBuffer, indexBuffer } );
    
    auto diffuseIter = this->textures.find( node->material.diffuseTexture[0] );

    CombinedImageSamplerDescriptor texture { diffuseIter->second->getImageView( ), diffuseIter->second->getSampler( ) };
    model->rsDescriptorSets.update( { texture } );
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
      rayTrace( );
    }
    else
    {
      rasterize( );
    }
  }

  void Api::rasterize( )
  {
    std::map<std::string, uint32_t> temp;
    for ( const auto& model : this->models )
    {
      temp.emplace( model.first, 0 );
    }

    for ( const auto& node : this->geometryNodes )
    {
      if ( node->modelPath.empty( ) )
        continue;

      auto it = this->models.find( node->modelPath );
      RX_ASSERT( ( it != this->models.end( ) ), "Can not draw model because it was not found." );

      auto it2 = temp.find( it->first );
      if ( it2 != temp.end( ) )
      {
        ++( it2->second );
      }
    }

    // Set up render pass begin info
    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color = { Util::vec4toArray( this->settings->getClearColor( ) ) };
    clearValues[1].depthStencil = vk::ClearDepthStencilValue { 1.0f, 0 };

    // Start recording the swapchain framebuffers
    for ( size_t imageIndex = 0; imageIndex < this->swapchainCommandBuffers.get( ).size( ); ++imageIndex )
    {
      this->swapchainCommandBuffers.begin( imageIndex );

      this->renderPass.begin( this->swapchain.getFramebuffer( imageIndex ),
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
      for ( const auto& it : this->models )
      {
        auto model = it.second;

        uint32_t instanceCount = 1;
        auto it2 = temp.find( it.first );
        if ( it2 != temp.end( ) )
          instanceCount = it2->second;

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
        std::vector<vk::DescriptorSet> descriptorSets = { this->rsSceneDescriptorSets.get( imageIndex ), model->rsDescriptorSets.get( imageIndex ) };

        this->swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eGraphics,
                                                                            rsPipeline.getLayout( ),
                                                                            0,                                               // first set
                                                                            static_cast<uint32_t>( descriptorSets.size( ) ), // descriptor set count
                                                                            descriptorSets.data( ),                          // descriptor sets
                                                                            0,                                               // dynamic offset count
                                                                            nullptr );                                       // dynamic offsets 

        this->swapchainCommandBuffers.get( imageIndex ).drawIndexed( model->indexBuffer.getCount( ), // index count
                                                                     instanceCount,                  // instance count
                                                                     0,                              // first index
                                                                     0,                              // vertex offset
                                                                     0 );                            // first instance
      }

      this->renderPass.end( this->swapchainCommandBuffers.get( imageIndex ) );
      this->swapchainCommandBuffers.end( imageIndex );
    }
  }

  void Api::rayTrace( )
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

        std::vector<vk::DescriptorSet> descriptorSets = { this->rtDescriptorSets.get( imageIndex ), iter->second->rtDescriptorSets.get( imageIndex ), this->rtSceneDescriptorSets.get( imageIndex ), this->vertexDataDescriptorSets.get( imageIndex ) };

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
      // Vertex buffer
      auto vertexBufferBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      // Index buffer                                 
      auto indexBufferBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      this->rtModelDescriptorSetLayout.init( { vertexBufferBinding, indexBufferBinding } );

      //this->rtModelBindings.add( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      //this->rtModelBindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
    }

    // RT Scene descriptor set layout.
    {
      // Light nodes uniform buffer
      auto lightsUboBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Scene description buffer
      auto rayTracingInstancesBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      this->rtSceneDescriptorSetLayout.init( { lightsUboBinding, rayTracingInstancesBinding } );
    }

    // RS Scene descriptor set layout.
    {
      // Uniform buffer binding for the vertex shader.
      auto cameraUboBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex );

      // Light nodes uniform buffer
      auto lightsUboBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment );

      // Scene description buffer
      auto rayTracingInstancesBinding = vk::Helper::getDescriptorSetLayoutBinding( 2, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eVertex );

      this->rsSceneDescriptorSetLayout.init( { cameraUboBinding, lightsUboBinding, rayTracingInstancesBinding } );
    }

    // Rasterization descriptor set layout.
    {
      // Texture image
      auto textureBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment );

      this->rsModelDescriptorSetLayout.init( { textureBinding } );
    }

    // Ray tracing descriptor pool
    this->rtDescriptorPool = vk::Initializer::initDescriptorPoolUnique( this->rtDescriptorSetLayout.getBindings( ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    // Model descriptor pool
    this->rtModelDescriptorPool = vk::Initializer::initDescriptorPoolUnique( this->rtModelDescriptorSetLayout.getBindings( ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );
    this->rsModelDescriptorPool = vk::Initializer::initDescriptorPoolUnique( this->rsModelDescriptorSetLayout.getBindings( ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );
    
    // Scene descriptor pool
    this->rtSceneDescriptorPool = vk::Initializer::initDescriptorPoolUnique( this->rtSceneDescriptorSetLayout.getBindings( ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );
    this->rsSceneDescriptorPool = vk::Initializer::initDescriptorPoolUnique( this->rsSceneDescriptorSetLayout.getBindings( ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    // Ray tracing descriptor sets
    this->rtDescriptorSets.init( this->rtDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rtDescriptorSetLayout.get( ) } );

    // Scene descriptor sets
    this->rtSceneDescriptorSets.init( this->rtSceneDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rtSceneDescriptorSetLayout.get( ) } );
    this->rsSceneDescriptorSets.init( this->rsSceneDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rsSceneDescriptorSetLayout.get( ) } );

    // Uniform buffers for camera
    this->cameraUniformBuffer.init<CameraUbo>( static_cast<uint32_t>( g_swapchainImageCount ) );

    // Uniform buffers for light nodes
    this->lightsUniformBuffer.init<LightsUbo>( static_cast<uint32_t>( g_swapchainImageCount ) );
  }

  void Api::initSceneStorageBuffer( )
  {
    // SSBO for scene description
    uint32_t maxInstanceCount = this->settings->anticipatedGeometryNodes.has_value( ) ? this->settings->anticipatedGeometryNodes.value( ) : g_maxGeometryNodes;
    this->rtInstances.resize( maxInstanceCount );
    this->rayTracingInstancesBuffer.init<RayTracingInstance>( this->rtInstances );
    this->rtInstances.clear( );

    UboDescriptor cameraUboDescriptor { this->cameraUniformBuffer.getRaw( ), sizeof( CameraUbo ) };
    UboDescriptor lightsUboDescriptor { this->lightsUniformBuffer.getRaw( ), sizeof( LightsUbo ) };
    StorageBufferDescriptor rtInstancesDescriptor { this->rayTracingInstancesBuffer.get( ) };

    this->rtSceneDescriptorSets.update( { lightsUboDescriptor, rtInstancesDescriptor } );
    this->rsSceneDescriptorSets.update( { cameraUboDescriptor, lightsUboDescriptor, rtInstancesDescriptor } );
  }

  void Api::updateSettings( )
  {
    if ( this->settings->refreshPipeline )
    {
      this->settings->refreshPipeline = false;
      this->settings->refreshSwapchain = false;

      g_device.waitIdle( );

      initPipelines( );
      this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );

      recreateSwapchain( );
    }

    if ( this->settings->refreshSwapchain )
    {
      this->settings->refreshSwapchain = false;

      recreateSwapchain( );
    }
  }

  void Api::updateUniformBuffers( )
  {
    uint32_t imageIndex = this->swapchain.getCurrentImageIndex( );

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

      this->cameraUbo.position = this->camera->getPosition( );
    }
    this->cameraUniformBuffer.upload<CameraUbo>( imageIndex, this->cameraUbo );

    // Upload lights
    LightsUbo lightNodeUbos;

    size_t i = 0;
    for ( auto iter = this->dirLightNodes.begin( ); iter != this->dirLightNodes.end( ); ++iter )
    {
      lightNodeUbos.directionalLightNodes[i] = ( *iter )->toUbo( );
      ++i;
    }

    i = 0;
    for ( auto iter = this->pointLightNodes.begin( ); iter != this->pointLightNodes.end( ); ++iter )
    {
      lightNodeUbos.pointLightNodes[i] = ( *iter )->toUbo( );
      ++i;
    }

    this->lightsUniformBuffer.upload<LightsUbo>( imageIndex, lightNodeUbos );
  }

  void Api::setModels( const std::vector<std::string>& modelPaths )
  {
    this->modelPaths = modelPaths;
    this->models.clear( );

    for ( const auto& path : modelPaths )
    {
      auto model = std::make_shared<Model>( path );
      this->models.insert( { path, model } );

      model->vertexBuffer.init( model->vertices );
      model->indexBuffer.init( model->indices );
      model->initialized = true;

      model->rtDescriptorSets.init( this->rtModelDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rtModelDescriptorSetLayout.get( ) } );
      model->rsDescriptorSets.init( this->rsModelDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->rsModelDescriptorSetLayout.get( ) } );
    }

    // modelDataDescSet
    uint32_t descriptorCount = static_cast<uint32_t>( this->models.size( ) );
    auto verticesBinding = vk::Helper::getDescriptorSetLayoutBinding( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, descriptorCount );
    auto indicesBinding = vk::Helper::getDescriptorSetLayoutBinding( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, descriptorCount );

    this->vertexDataSetLayout.init( { verticesBinding, indicesBinding } );
    this->vertexDataDescriptorPool = vk::Initializer::initDescriptorPoolUnique( this->vertexDataSetLayout.getBindings( ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    this->vertexDataDescriptorSets.init( this->vertexDataDescriptorPool.get( ), g_swapchainImageCount, std::vector<vk::DescriptorSetLayout>{ g_swapchainImageCount, this->vertexDataSetLayout.get( ) } );

    StorageBufferArrayDescriptor verticesDescriptors;
    verticesDescriptors.storageBuffers.reserve( this->models.size( ) );

    StorageBufferArrayDescriptor indicesDescriptors;
    indicesDescriptors.storageBuffers.reserve( this->models.size( ) );

    for ( const auto& model : this->models )
    {
      verticesDescriptors.storageBuffers.push_back( { model.second->vertexBuffer.get( ) } );
      indicesDescriptors.storageBuffers.push_back( { model.second->indexBuffer.get( ) } );
    }

    this->vertexDataDescriptorSets.update( { verticesDescriptors, indicesDescriptors } );

    initPipelines( );
  }
}
