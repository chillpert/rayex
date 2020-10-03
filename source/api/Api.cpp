#include "api/Api.hpp"

#include "api/misc/Components.hpp"
#include "api/utility/Destructors.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Util.hpp"

#include <utility>

namespace RAYEXEC_NAMESPACE
{
  const std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };
#ifdef RX_DEBUG
  std::vector<const char*> extensions = { "VK_KHR_get_physical_device_properties2", "VK_EXT_debug_utils" };
#else
  std::vector<const char*> extensions = { "VK_KHR_get_physical_device_properties2" };
#endif

  std::vector<const char*> deviceExtensions = {
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
    window( std::move( window ) ),
    camera( std::move( camera ) ),
    gui( nullptr )
  {
  }

  Api::Api( std::shared_ptr<Window> window, std::shared_ptr<Gui> gui, std::shared_ptr<Camera> camera ) :
    window( std::move( window ) ),
    camera( std::move( camera ) ),
    gui( std::move( gui ) )
  {
  }

  Api::~Api( )
  {
    g_device.waitIdle( );

    // Gui needs to be destroyed manually, as RAII destruction will not be possible.
    if ( this->gui != nullptr )
    {
      this->gui->destroy( );
    }
  }

  void Api::setGui( const std::shared_ptr<Gui>& gui, bool initialize )
  {
    if ( this->gui != nullptr )
    {
      recreateSwapchain( );
      this->gui->destroy( );
    }

    this->gui = gui;

    if ( initialize )
    {
      initGui( );
    }
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
    this->physicalDevice = vk::Initializer::initPhysicalDevice( );

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
    g_graphicsCmdPool     = this->graphicsCmdPool.get( );

    this->transferCmdPool = vk::Initializer::initCommandPoolUnique( g_transferFamilyIndex, { } );
    g_transferCmdPool     = this->transferCmdPool.get( );

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

    // GUI
    initGui( );

    // Init and record swapchain command buffers.
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

  auto Api::prepareFrame( ) -> bool
  {
    // Wait for the current frame's fences.
    vk::Result result = g_device.waitForFences( 1, &this->inFlightFences[currentFrame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

    // If the window is minimized then simply do not render anything anymore.
    if ( this->window->minimized( ) )
    {
      return true;
    }

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

  auto Api::submitFrame( ) -> bool
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
    {
      commandBuffers.push_back( this->gui->getCommandBuffer( imageIndex ) );
    }

    // Reset the signaled state of the current frame's fence to the unsignaled one.
    g_device.resetFences( 1, &this->inFlightFences[currentFrame].get( ) );

    // Submits / executes the current image's / framebuffer's command buffer.
    auto pWaitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    auto submitInfo        = vk::Helper::getSubmitInfo( this->imageAvailableSemaphores[currentFrame].get( ), this->finishedRenderSemaphores[currentFrame].get( ), commandBuffers, pWaitDstStageMask );
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

  auto Api::render( ) -> bool
  {
    if ( this->gui != nullptr )
    {
      this->gui->newFrame( );
      this->gui->render( );
      this->gui->endRender( );
    }

    if ( prepareFrame( ) )
    {
      return true;
    }

    if ( this->gui != nullptr )
    {
      this->gui->renderDrawData( this->swapchain.getCurrentImageIndex( ) );
    }

    if ( submitFrame( ) )
    {
      return true;
    }

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

    vk::WriteDescriptorSetAccelerationStructureKHR tlasInfo( 1,
                                                             &this->rayTracingBuilder.getTlas( ).as.as );

    vk::DescriptorImageInfo storageImageInfo( nullptr,
                                              this->rayTracingBuilder.getStorageImageView( ),
                                              vk::ImageLayout::eGeneral );

    this->rtBindings.write( this->rtDescriptorSets, 0, &tlasInfo );
    this->rtBindings.write( this->rtDescriptorSets, 1, &storageImageInfo );
    this->rtBindings.write( this->rtDescriptorSets, 2, this->cameraUniformBuffer.bufferInfos );
    this->rtBindings.update( );

    // Swapchain command buffers
    recordSwapchainCommandBuffers( );

    if ( this->gui != nullptr )
    {
      this->gui->recreate( this->swapchain.getExtent( ), this->swapchain.getImageViews( ) );
    }

    // Update the camera screen size to avoid image stretching.
    auto screenSize = this->swapchain.getExtent( );
    this->camera->setSize( screenSize.width, screenSize.height );

    RX_SUCCESS( "Swapchain recreation finished." );
  }

  void Api::popNode( const std::shared_ptr<Node>& node )
  {
    if ( node == nullptr )
    {
      RX_ERROR( "Can not pop node because node is nullptr." );
      return;
    }

    if ( node->getType( ) == NodeType::eGeometryNode )
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
    else if ( node->getType( ) == NodeType::eLightNode )
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
    vk::WriteDescriptorSetAccelerationStructureKHR tlasInfo( 1,
                                                             &this->rayTracingBuilder.getTlas( ).as.as );

    vk::DescriptorImageInfo storageImageInfo( nullptr,
                                              this->rayTracingBuilder.getStorageImageView( ),
                                              vk::ImageLayout::eGeneral );

    this->rtBindings.write( this->rtDescriptorSets, 0, &tlasInfo );
    this->rtBindings.write( this->rtDescriptorSets, 1, &storageImageInfo );
    this->rtBindings.write( this->rtDescriptorSets, 2, this->cameraUniformBuffer.bufferInfos );
    this->rtBindings.update( );
  }

  void Api::initPipelines( )
  {
    // Ray tracing pipeline
    std::vector<vk::DescriptorSetLayout> temp = { this->rtDescriptorSetLayout.get( ), this->rtModelDescriptorSetLayout.get( ), this->rtSceneDescriptorSetLayout.get( ) };
    this->rtPipeline.init( temp, this->settings );

    // Rasterization pipeline
    glm::vec2 extent = { static_cast<float>( this->swapchain.getExtent( ).width ), static_cast<float>( this->swapchain.getExtent( ).height ) };
    this->viewport   = vk::Viewport( 0.0F, 0.0F, extent.x, extent.y, 0.0F, 1.0F );
    this->scissor    = vk::Rect2D( 0, this->swapchain.getExtent( ) );
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
  void Api::initModel( const std::shared_ptr<GeometryNode>& node )
  {
    auto it = this->models.find( node->modelPath );
    RX_ASSERT( it != this->models.end( ), "Failed to find model." );
    auto model = it->second;

    updateAccelerationStructure( );

    // Model RS descriptor set update.
    // This is only here because textures are as of now only part of a Material object which each node holds. That's why this is initilized here and not in setModels( ).
    auto diffuseIter = this->textures.find( node->material.diffuseTexture[0] );

    vk::DescriptorImageInfo textureInfo( diffuseIter->second->getSampler( ),
                                         diffuseIter->second->getImageView( ),
                                         vk::ImageLayout::eShaderReadOnlyOptimal );

    this->rsModelBindings.write( model->rsDescriptorSets, 0, &textureInfo );
    this->rsModelBindings.update( );
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
    this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );

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
      {
        continue;
      }

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
    clearValues[0].color        = { Util::vec4toArray( this->settings->getClearColor( ) ) };
    clearValues[1].depthStencil = vk::ClearDepthStencilValue { 1.0F, 0 };

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
      viewport.width        = this->window->getWidth( );
      viewport.height       = this->window->getHeight( );

      this->swapchainCommandBuffers.get( imageIndex ).setViewport( 0, 1, &viewport ); // CMD

      vk::Rect2D scissor = this->scissor;
      scissor.extent     = this->window->getExtent( );

      this->swapchainCommandBuffers.get( imageIndex ).setScissor( 0, 1, &scissor ); // CMD

      // Draw models
      for ( const auto& it : this->models )
      {
        auto model = it.second;

        uint32_t instanceCount = 1;
        auto it2               = temp.find( it.first );
        if ( it2 != temp.end( ) )
        {
          instanceCount = it2->second;
        }

        std::array<vk::Buffer, 1> vertexBuffers { model->vertexBuffer.get( ) };
        std::array<vk::DeviceSize, 1> offsets { 0 };

        this->swapchainCommandBuffers.get( imageIndex ).bindVertexBuffers( 0,                     // first binding
                                                                           1,                     // binding count
                                                                           vertexBuffers.data( ), // pBuffers
                                                                           offsets.data( ) );     // pOffsets

        this->swapchainCommandBuffers.get( imageIndex ).bindIndexBuffer( model->indexBuffer.get( ),
                                                                         0, // offset
                                                                         model->indexBuffer.getType( ) );

        // TODO: Also bind ray tracing descriptor set.
        std::vector<vk::DescriptorSet> descriptorSets = { this->rsSceneDescriptorSets[imageIndex], model->rsDescriptorSets[imageIndex] };

        this->swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eGraphics, rsPipeline.getLayout( ),
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

        std::vector<vk::DescriptorSet> descriptorSets = { this->rtDescriptorSets[imageIndex], iter->second->rtDescriptorSets[imageIndex], this->rtSceneDescriptorSets[imageIndex] };

        this->swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eRayTracingKHR, this->rtPipeline.getLayout( ),
                                                                            0,                                               // first set
                                                                            static_cast<uint32_t>( descriptorSets.size( ) ), // descriptor set count
                                                                            descriptorSets.data( ),                          // descriptor sets
                                                                            0,                                               // dynamic offset count
                                                                            nullptr );                                       // dynamic offsets

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
      this->inFlightFences[i]           = vk::Initializer::initFenceUnique( );
    }
  }

  void Api::initDescriptorSets( )
  {
    // Create the ray tracing descriptor set layout
    {
      // TLAS
      this->rtBindings.add( 0, vk::DescriptorType::eAccelerationStructureKHR, vk::ShaderStageFlagBits::eRaygenKHR );
      // Output image
      this->rtBindings.add( 1, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eRaygenKHR );
      // Camera uniform buffer
      this->rtBindings.add( 2, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eRaygenKHR );

      this->rtDescriptorSetLayout = this->rtBindings.initLayoutUnique( );
      this->rtDescriptorPool      = this->rtBindings.initPoolUnique( g_swapchainImageCount );
      this->rtDescriptorSets      = vk::Initializer::initDescriptorSetsUnique( this->rtDescriptorPool, this->rtDescriptorSetLayout );
    }

    // Create the descriptor set layout for models
    {
      // Vertex buffer
      this->rtModelBindings.add( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Index buffer
      this->rtModelBindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      this->rtModelDescriptorSetLayout = this->rtModelBindings.initLayoutUnique( );
      this->rtModelDescriptorPool      = this->rtModelBindings.initPoolUnique( static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );
    }

    // RT Scene descriptor set layout.
    {
      // Light nodes uniform buffer
      this->rtSceneBindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Scene description buffer
      this->rtSceneBindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      this->rtSceneDescriptorSetLayout = this->rtSceneBindings.initLayoutUnique( );
      this->rtSceneDescriptorPool      = this->rtSceneBindings.initPoolUnique( g_swapchainImageCount );
      this->rtSceneDescriptorSets      = vk::Initializer::initDescriptorSetsUnique( this->rtSceneDescriptorPool, this->rtSceneDescriptorSetLayout );
    }

    // RS Scene descriptor set layout.
    {
      // Uniform buffer binding for the vertex shader.
      this->rsSceneBindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex );
      // Light nodes uniform buffer
      this->rsSceneBindings.add( 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment );
      // Scene description buffer
      this->rsSceneBindings.add( 2, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eVertex );

      this->rsSceneDescriptorSetLayout = this->rsSceneBindings.initLayoutUnique( );
      this->rsSceneDescriptorPool      = this->rsSceneBindings.initPoolUnique( g_swapchainImageCount );
      this->rsSceneDescriptorSets      = vk::Initializer::initDescriptorSetsUnique( this->rsSceneDescriptorPool, this->rsSceneDescriptorSetLayout );
    }

    // Rasterization descriptor set layout.
    {
      // Texture image
      this->rsModelBindings.add( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment );

      this->rsModelDescriptorSetLayout = this->rsModelBindings.initLayoutUnique( );
      this->rsModelDescriptorPool      = this->rsModelBindings.initPoolUnique( static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );
    }

    // Uniform buffers for camera
    this->cameraUniformBuffer.init<CameraUbo>( );

    // Uniform buffers for light nodes
    this->lightsUniformBuffer.init<LightsUbo>( );
  }

  void Api::initSceneStorageBuffer( )
  {
    // SSBO for scene description
    uint32_t maxInstanceCount = this->settings->anticipatedGeometryNodes.has_value( ) ? this->settings->anticipatedGeometryNodes.value( ) : g_maxGeometryNodes;
    this->rtInstances.resize( maxInstanceCount );
    this->rayTracingInstancesBuffer.init<RayTracingInstance>( this->rtInstances );
    this->rtInstances.clear( );

    // Update RT scene descriptor sets.
    vk::DescriptorBufferInfo rtInstancesInfo( this->rayTracingInstancesBuffer.get( ),
                                              0,
                                              VK_WHOLE_SIZE );

    this->rtSceneBindings.write( this->rtSceneDescriptorSets, 0, this->lightsUniformBuffer.bufferInfos );
    this->rtSceneBindings.write( this->rtSceneDescriptorSets, 1, &rtInstancesInfo );
    this->rtSceneBindings.update( );

    // Update RS scene descriptor sets.
    this->rsSceneBindings.write( this->rsSceneDescriptorSets, 0, this->cameraUniformBuffer.bufferInfos );
    this->rsSceneBindings.write( this->rsSceneDescriptorSets, 1, this->lightsUniformBuffer.bufferInfos );
    this->rsSceneBindings.write( this->rsSceneDescriptorSets, 2, &rtInstancesInfo );
    this->rsSceneBindings.update( );
  }

  void Api::updateSettings( )
  {
    if ( this->settings->refreshPipeline )
    {
      this->settings->refreshPipeline  = false;
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
        this->cameraUbo.view        = this->camera->getViewMatrix( );
        this->cameraUbo.viewInverse = this->camera->getViewInverseMatrix( );

        this->camera->updateView = false;
      }

      if ( this->camera->updateProj )
      {
        this->cameraUbo.projection        = this->camera->getProjectionMatrix( );
        this->cameraUbo.projectionInverse = this->camera->getProjectionInverseMatrix( );

        this->camera->updateProj = false;
      }

      this->cameraUbo.position = this->camera->getPosition( );
    }
    this->cameraUniformBuffer.upload<CameraUbo>( imageIndex, this->cameraUbo );

    // Upload lights
    LightsUbo lightNodeUbos;

    size_t i = 0;
    for ( auto& dirLightNode : this->dirLightNodes )
    {
      lightNodeUbos.directionalLightNodes[i] = dirLightNode->toUbo( );
      ++i;
    }

    i = 0;
    for ( auto& pointLightNode : this->pointLightNodes )
    {
      lightNodeUbos.pointLightNodes[i] = pointLightNode->toUbo( );
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

      model->rtDescriptorSets = vk::Initializer::initDescriptorSetsUnique( this->rtModelDescriptorPool, this->rtModelDescriptorSetLayout );

      vk::DescriptorBufferInfo vertexBufferInfo( model->vertexBuffer.get( ),
                                                 0,
                                                 VK_WHOLE_SIZE );

      vk::DescriptorBufferInfo indexBufferInfo( model->indexBuffer.get( ),
                                                0,
                                                VK_WHOLE_SIZE );

      this->rtModelBindings.write( model->rtDescriptorSets, 0, &vertexBufferInfo );
      this->rtModelBindings.write( model->rtDescriptorSets, 1, &indexBufferInfo );
      this->rtModelBindings.update( );

      model->rsDescriptorSets = vk::Initializer::initDescriptorSetsUnique( this->rsModelDescriptorPool, this->rsModelDescriptorSetLayout );
    }

    //initPipelines( );
  }
} // namespace RAYEXEC_NAMESPACE
