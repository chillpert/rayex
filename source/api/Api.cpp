#include "api/Api.hpp"

#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Util.hpp"

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
  size_t prevFrame    = 0;

  std::vector<GeometryInstance> dereferencedGeometryInstances;
  std::vector<DirectionalLightSSBO> dereferencedDirectionalLights;

  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  Api::Api( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera ) :
    window( std::move( window ) ),
    camera( std::move( camera ) ),
    gui( nullptr )
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

  void Api::initBase( )
  {
    // Instance
    instance = vk::Initializer::initInstance( layers, extensions );

    // Debug messenger
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
    this->settings->refreshSwapchain = false;

    // Command pools
    this->graphicsCmdPool = vk::Initializer::initCommandPoolUnique( g_graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
    g_graphicsCmdPool     = this->graphicsCmdPool.get( );

    this->transferCmdPool = vk::Initializer::initCommandPoolUnique( g_transferFamilyIndex, { } );
    g_transferCmdPool     = this->transferCmdPool.get( );

    // GUI
    initGui( );

    // Create fences and semaphores.
    initSyncObjects( );

    // At this point nodes are unknown. So I just create a storage buffer with the maximum value or with the anticipated value in settings.
    // enables the user to add more nodes later on during runtime.
    uint32_t maxInstanceCount = this->settings->maxGeometryInstances.has_value( ) ? this->settings->maxGeometryInstances.value( ) : g_maxGeometryInstances;

    std::vector<GeometryInstance> geometryInstances( maxInstanceCount );
    this->geometryInstancesBuffer.init<GeometryInstance>( geometryInstances );

    uint32_t maxDirectionalLightCount = this->settings->maxDirectionalLights.has_value( ) ? this->settings->maxDirectionalLights.value( ) : g_maxDirectionalLights;
    std::vector<DirectionalLightSSBO> directionalLights( maxDirectionalLightCount );
    this->directionalLightsBuffer.init<DirectionalLightSSBO>( directionalLights );
  }

  void Api::initScene( )
  {
    // Uniform buffers for camera
    this->cameraUniformBuffer.init<CameraUbo>( );

    // Init geometry storage buffers.
    g_modelCount = static_cast<uint32_t>( this->scene.geometries.size( ) );

    this->vertexBuffers.resize( this->scene.geometries.size( ) );
    this->indexBuffers.resize( this->scene.geometries.size( ) );
    this->meshBuffers.resize( this->scene.geometries.size( ) );

    for ( size_t i = 0; i < this->scene.geometries.size( ); ++i )
    {
      this->vertexBuffers[i].init( this->scene.geometries[i]->vertices );
      this->indexBuffers[i].init( this->scene.geometries[i]->indices );
      this->meshBuffers[i].init( this->scene.geometries[i]->meshes );
    }

    // Descriptor sets and layouts
    initDescriptorSets( );

    // Update RT scene descriptor sets.
    updateSceneDescriptors( );

    // Update the vertex and index SSBO descriptors for the ray tracing shaders.
    updateRayTracingModelData( );

    // Initialize a rasterization and raytracing pipeline.
    initPipelines( );

    // Ray tracing
    this->rayTracingBuilder.init( );
    this->rayTracingBuilder.createStorageImage( this->swapchain.getExtent( ) );
    this->rayTracingBuilder.createShaderBindingTable( this->rtPipeline.get( ) );

    this->settings->maxRecursionDepth = this->rayTracingBuilder.getRtProperties( ).maxRecursionDepth;

    updateAccelerationStructures( );

    // Init and record swapchain command buffers.
    //this->swapchainCommandBuffers.reset( );
    this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
    recordSwapchainCommandBuffers( );
  }

  void Api::update( )
  {
    updateSettings( );

    updateUniformBuffers( );

    if ( this->uploadGeometryInstancesToBuffer )
    {
      this->uploadGeometryInstancesToBuffer = false;

      if ( this->scene.geometryInstances.size( ) > 0 )
      {
        // Dereference pointers and store values in new vector that will be uploaded.
        dereferencedGeometryInstances.resize( this->scene.geometryInstances.size( ) );
        std::transform( this->scene.geometryInstances.begin( ),
                        this->scene.geometryInstances.end( ),
                        dereferencedGeometryInstances.begin( ),
                        []( std::shared_ptr<GeometryInstance> instance ) { return *instance; } );

        this->geometryInstancesBuffer.fill<GeometryInstance>( dereferencedGeometryInstances.data( ) );
        updateAccelerationStructures( );
      }
    }

    if ( this->uploadDirectionalLightsToBuffer )
    {
      this->uploadDirectionalLightsToBuffer = false;

      if ( this->scene.directionalLights.size( ) > 0 )
      {
        dereferencedDirectionalLights.resize( this->scene.directionalLights.size( ) );
        std::transform( this->scene.directionalLights.begin( ),
                        this->scene.directionalLights.end( ),
                        dereferencedDirectionalLights.begin( ),
                        []( std::shared_ptr<DirectionalLight> light ) { return DirectionalLightSSBO { glm::vec4( light->ambient, light->ambientIntensity ),
                                                                                                      glm::vec4( light->diffuse, light->diffuseIntensity ),
                                                                                                      glm::vec4( light->specular, light->specularIntensity ),
                                                                                                      glm::vec4( light->direction, 1.0F ) }; } );

        this->directionalLightsBuffer.fill<DirectionalLightSSBO>( dereferencedDirectionalLights.data( ) );
      }
    }

    if ( this->settings->getJitterCamEnabled( ) )
    {
      if ( g_frameCount >= this->settings->jitterCamSampleRate )
        return;

      // Increment frame counter for jitter cam.
      ++g_frameCount;
    }
  }

  auto Api::prepareFrame( ) -> bool
  {
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

    // Wait for the current frame's fences.
    vk::Result result = g_device.waitForFences( 1, &this->inFlightFences[currentFrame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

    return false;
  }

  auto Api::submitFrame( ) -> bool
  {
    uint32_t imageIndex = this->swapchain.getCurrentImageIndex( );

    // Check if a previous frame is using the current image.
    if ( this->imagesInFlight[imageIndex] )
    {
      vk::Result result = g_device.waitForFences( 1, &this->imagesInFlight[currentFrame], VK_TRUE, UINT64_MAX );
      RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );
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

    prevFrame    = currentFrame;
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

    // Wait for previous frame to finish command buffer execution.
    vk::Result result = g_device.waitForFences( 1, &this->inFlightFences[prevFrame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );
    recordSwapchainCommandBuffers( );

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
    RX_LOG_TIME_START( "Re-creating swapchain ..." );

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

    this->rtDescriptors.bindings.write( this->rtDescriptorSets, 0, &tlasInfo );
    this->rtDescriptors.bindings.write( this->rtDescriptorSets, 1, &storageImageInfo );
    this->rtDescriptors.bindings.update( );

    // Swapchain command buffers
    this->swapchainCommandBuffers.init( this->graphicsCmdPool.get( ), g_swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
    recordSwapchainCommandBuffers( );

    if ( this->gui != nullptr )
    {
      this->gui->recreate( this->swapchain.getExtent( ), this->swapchain.getImageViews( ) );
    }

    // Update the camera screen size to avoid image stretching.
    auto screenSize = this->swapchain.getExtent( );
    this->camera->setSize( screenSize.width, screenSize.height );

    this->settings->refreshSwapchain = false;

    RX_LOG_TIME_STOP( "Finished re-creating swapchain" );
  }

  void Api::updateAccelerationStructures( )
  {
    RX_LOG_TIME_START( "Updating acceleration structures ..." );

    // @TODO Try to call this as few times as possible.
    this->rayTracingBuilder.createBottomLevelAS( this->vertexBuffers, this->indexBuffers );
    this->rayTracingBuilder.createTopLevelAS( this->scene.geometryInstances );

    // Update ray tracing descriptor set.
    vk::WriteDescriptorSetAccelerationStructureKHR tlasInfo( 1,
                                                             &this->rayTracingBuilder.getTlas( ).as.as );

    vk::DescriptorImageInfo storageImageInfo( nullptr,
                                              this->rayTracingBuilder.getStorageImageView( ),
                                              vk::ImageLayout::eGeneral );

    this->rtDescriptors.bindings.write( this->rtDescriptorSets, 0, &tlasInfo );
    this->rtDescriptors.bindings.write( this->rtDescriptorSets, 1, &storageImageInfo );
    this->rtDescriptors.bindings.update( );

    RX_LOG_TIME_STOP( "Finished updating acceleration structures" );
  }

  void Api::initPipelines( )
  {
    RX_LOG_TIME_START( "Initializing graphic pipelines ..." );

    // Ray tracing pipeline
    std::vector<vk::DescriptorSetLayout> allRtDescriptorSetLayouts = { this->rtDescriptors.layout.get( ),
                                                                       this->rtSceneDescriptors.layout.get( ),
                                                                       this->vertexDataDescriptors.layout.get( ),
                                                                       this->indexDataDescriptors.layout.get( ) };
    this->rtPipeline.init( allRtDescriptorSetLayouts, this->settings );

    // Rasterization pipeline
    glm::vec2 extent = { static_cast<float>( this->swapchain.getExtent( ).width ), static_cast<float>( this->swapchain.getExtent( ).height ) };
    this->viewport   = vk::Viewport( 0.0F, 0.0F, extent.x, extent.y, 0.0F, 1.0F );
    this->scissor    = vk::Rect2D( 0, this->swapchain.getExtent( ) );

    std::vector<vk::DescriptorSetLayout> allRsDescriptorSetLayouts = { this->rsSceneDescriptors.layout.get( ) };

    this->rsPipeline.init( allRsDescriptorSetLayouts, this->renderPass.get( ), viewport, scissor, this->settings );

    this->pipelinesReady            = true;
    this->settings->refreshPipeline = false;

    RX_LOG_TIME_STOP( "Finished graphic pipelines initialization" );
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
    RX_ASSERT( this->pipelinesReady, "Can not record swapchain command buffers because the pipelines have not been initialized yet." );

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
    std::map<std::string_view, uint32_t> temp;

    for ( auto geometry : this->scene.geometries )
    {
      temp.emplace( geometry->path, 0 );
    }

    for ( const auto& geometryInstance : this->scene.geometryInstances )
    {
      std::shared_ptr<Geometry> it = findGeometry( geometryInstance->geometryIndex );
      RX_ASSERT( it != nullptr, "Could not find model. Did you forget to introduce the renderer to this model using RayExec::setModels( ) after initializing the renderer?" );

      auto it2 = temp.find( it->path );
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
      viewport.width        = static_cast<float>( this->window->getWidth( ) );
      viewport.height       = static_cast<float>( this->window->getHeight( ) );

      this->swapchainCommandBuffers.get( imageIndex ).setViewport( 0, 1, &viewport ); // CMD

      vk::Rect2D scissor = this->scissor;
      scissor.extent     = this->window->getExtent( );

      this->swapchainCommandBuffers.get( imageIndex ).setScissor( 0, 1, &scissor ); // CMD

      // Draw models
      uint32_t id = 0;
      for ( const auto& geometryInstance : this->scene.geometryInstances )
      {
        auto geo = findGeometry( geometryInstance->geometryIndex );
        RX_ASSERT( geo != nullptr, "Could not find model. Did you forget to introduce the renderer to this model using RayExec::setModels( ) after initializing the renderer?" );

        uint32_t instanceCount = 1;
        auto it2               = temp.find( geo->path );
        if ( it2 != temp.end( ) )
        {
          instanceCount = it2->second;

          if ( instanceCount == 0 )
          {
            continue;
          }
        }

        this->swapchainCommandBuffers.get( imageIndex ).pushConstants( this->rsPipeline.getLayout( ),    // layout
                                                                       vk::ShaderStageFlagBits::eVertex, // stageFlags
                                                                       0,                                // offset
                                                                       sizeof( uint32_t ),               // size
                                                                       &id );                            // pValues

        std::array<vk::Buffer, 1> vertexBuffers { this->vertexBuffers[geometryInstance->geometryIndex].get( ) };
        std::array<vk::DeviceSize, 1> offsets { 0 };

        this->swapchainCommandBuffers.get( imageIndex ).bindVertexBuffers( 0,                     // first binding
                                                                           1,                     // binding count
                                                                           vertexBuffers.data( ), // pBuffers
                                                                           offsets.data( ) );     // pOffsets

        this->swapchainCommandBuffers.get( imageIndex ).bindIndexBuffer( this->indexBuffers[geometryInstance->geometryIndex].get( ),
                                                                         0, // offset
                                                                         vk::IndexType::eUint32 );

        std::vector<vk::DescriptorSet> descriptorSets = { this->rsSceneDescriptorSets[imageIndex] };

        this->swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eGraphics, rsPipeline.getLayout( ),
                                                                            0,                                               // first set
                                                                            static_cast<uint32_t>( descriptorSets.size( ) ), // descriptor set count
                                                                            descriptorSets.data( ),                          // descriptor sets
                                                                            0,                                               // dynamic offset count
                                                                            nullptr );                                       // dynamic offsets

        this->swapchainCommandBuffers.get( imageIndex ).drawIndexed( this->indexBuffers[geometryInstance->geometryIndex].getCount( ), // index count
                                                                     instanceCount,                                                   // instance count
                                                                     0,                                                               // first index
                                                                     0,                                                               // vertex offset
                                                                     0 );                                                             // first instance

        ++id;
      }

      this->renderPass.end( this->swapchainCommandBuffers.get( imageIndex ) );
      this->swapchainCommandBuffers.end( imageIndex );
    }
  }

  void Api::rayTrace( )
  {
    uint32_t directionalLightCount = static_cast<uint32_t>( this->scene.directionalLights.size( ) );
    uint32_t pointLightCount       = static_cast<uint32_t>( this->scene.pointLights.size( ) );

    // Start recording the swapchain framebuffers
    for ( size_t imageIndex = 0; imageIndex < this->swapchainCommandBuffers.get( ).size( ); ++imageIndex )
    {
      this->swapchainCommandBuffers.begin( imageIndex );

      RayTracePushConstants chitPc = { this->settings->getClearColor( ),
                                       g_frameCount,
                                       this->settings->getJitterCamSampleRatePerRayGen( ),
                                       this->settings->getSsaaSampleRate( ),
                                       this->settings->getJitterCamEnabled( ),
                                       this->settings->getSsaaEnabled( ),
                                       directionalLightCount,
                                       pointLightCount };

      this->swapchainCommandBuffers.get( imageIndex ).pushConstants( this->rtPipeline.getLayout( ),                                                                                     // layout
                                                                     vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eMissKHR | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                                                     0,                                                                                                                 // offset
                                                                     sizeof( RayTracePushConstants ),                                                                                   // size
                                                                     &chitPc );                                                                                                         // pValues

      this->swapchainCommandBuffers.get( imageIndex ).bindPipeline( vk::PipelineBindPoint::eRayTracingKHR, this->rtPipeline.get( ) );

      std::vector<vk::DescriptorSet> descriptorSets = { this->rtDescriptorSets[imageIndex],
                                                        this->rtSceneDescriptorSets[imageIndex],
                                                        this->vertexDataDescriptorSets[imageIndex],
                                                        this->indexDataDescriptorSets[imageIndex] };

      this->swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eRayTracingKHR, this->rtPipeline.getLayout( ),
                                                                          0,                                               // first set
                                                                          static_cast<uint32_t>( descriptorSets.size( ) ), // descriptor set count
                                                                          descriptorSets.data( ),                          // descriptor sets
                                                                          0,                                               // dynamic offset count
                                                                          nullptr );                                       // dynamic offsets

      this->rayTracingBuilder.rayTrace( this->swapchainCommandBuffers.get( imageIndex ), this->swapchain.getImage( imageIndex ), this->swapchain.getExtent( ) );

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
      this->inFlightFences[i]           = vk::Initializer::initFenceUnique( vk::FenceCreateFlagBits::eSignaled );
    }
  }

  void Api::initDescriptorSets( )
  {
    // Create the ray tracing descriptor set layout
    {
      // TLAS
      this->rtDescriptors.bindings.add( 0, vk::DescriptorType::eAccelerationStructureKHR, vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR );
      // Output image
      this->rtDescriptors.bindings.add( 1, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eRaygenKHR );

      this->rtDescriptors.layout = this->rtDescriptors.bindings.initLayoutUnique( );
      this->rtDescriptors.pool   = this->rtDescriptors.bindings.initPoolUnique( g_swapchainImageCount );
      this->rtDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( this->rtDescriptors.pool, this->rtDescriptors.layout );
    }

    // RT Scene descriptor set layout.
    {
      // Camera uniform buffer
      this->rtSceneDescriptors.bindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eRaygenKHR );
      // Lights storage buffer
      this->rtSceneDescriptors.bindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Scene description buffer
      this->rtSceneDescriptors.bindings.add( 2, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Materials
      this->rtSceneDescriptors.bindings.add( 3, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, g_modelCount, vk::DescriptorBindingFlagBits::eVariableDescriptorCount );

      this->rtSceneDescriptors.layout = this->rtSceneDescriptors.bindings.initLayoutUnique( );
      this->rtSceneDescriptors.pool   = this->rtSceneDescriptors.bindings.initPoolUnique( g_swapchainImageCount );
      this->rtSceneDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( this->rtSceneDescriptors.pool, this->rtSceneDescriptors.layout );
    }

    // RS Scene descriptor set layout.
    {
      // Camera uniform buffer
      this->rsSceneDescriptors.bindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex );
      // Lights storage buffer
      this->rsSceneDescriptors.bindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment );
      // Scene description buffer
      this->rsSceneDescriptors.bindings.add( 2, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eVertex );

      this->rsSceneDescriptors.layout = this->rsSceneDescriptors.bindings.initLayoutUnique( );
      this->rsSceneDescriptors.pool   = this->rsSceneDescriptors.bindings.initPoolUnique( g_swapchainImageCount );
      this->rsSceneDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( this->rsSceneDescriptors.pool, this->rsSceneDescriptors.layout );
    }

    // Vertex model data descriptor set layout.
    {
      this->vertexDataDescriptors.bindings.add( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, g_modelCount, vk::DescriptorBindingFlagBits::eVariableDescriptorCount );

      this->vertexDataDescriptors.layout = this->vertexDataDescriptors.bindings.initLayoutUnique( );
      this->vertexDataDescriptors.pool   = this->vertexDataDescriptors.bindings.initPoolUnique( static_cast<uint32_t>( g_maxGeometryInstances ) * g_swapchainImageCount );
      this->vertexDataDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( this->vertexDataDescriptors.pool, this->vertexDataDescriptors.layout );
    }

    // Index model data descriptor set layout.
    {
      this->indexDataDescriptors.bindings.add( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, g_modelCount, vk::DescriptorBindingFlagBits::eVariableDescriptorCount );

      this->indexDataDescriptors.layout = this->indexDataDescriptors.bindings.initLayoutUnique( );
      this->indexDataDescriptors.pool   = this->indexDataDescriptors.bindings.initPoolUnique( static_cast<uint32_t>( g_maxGeometryInstances ) * g_swapchainImageCount );
      this->indexDataDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( this->indexDataDescriptors.pool, this->indexDataDescriptors.layout );
    }
  }

  void Api::updateSettings( )
  {
    if ( this->settings->refreshPipeline )
    {
      this->settings->refreshPipeline = false;

      g_device.waitIdle( );

#ifdef RX_COPY_ASSETS
      // Copies shader resources to binary output directory. This way a shader can be changed during runtime.
      // Make sure only to edit the ones in /assets/shaders and not in /build/bin/debug/assets/shaders as the latter gets overridden.
      RX_INFO( "Copying shader resources to binary output directory. " );
      std::filesystem::copy( RX_ASSETS_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
#endif

      initPipelines( );
      this->rayTracingBuilder.createShaderBindingTable( this->rtPipeline.get( ) );
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

    // Upload camera.
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

      this->cameraUbo.position = glm::vec4( this->camera->getPosition( ), 1.0F );
    }

    this->cameraUniformBuffer.upload<CameraUbo>( imageIndex, this->cameraUbo );
  }

  void Api::updateSceneDescriptors( )
  {
    vk::DescriptorBufferInfo rtInstancesInfo( this->geometryInstancesBuffer.get( ),
                                              0,
                                              VK_WHOLE_SIZE );

    vk::DescriptorBufferInfo directionalLightsInfo( this->directionalLightsBuffer.get( ),
                                                    0,
                                                    VK_WHOLE_SIZE );

    for ( const auto& buffer : this->meshBuffers )
    {
      vk::DescriptorBufferInfo meshDataBufferInfo( buffer.get( ),
                                                   0,
                                                   VK_WHOLE_SIZE );

      this->meshDataBufferInfos.push_back( meshDataBufferInfo );
    }

    this->rtSceneDescriptors.bindings.write( this->rtSceneDescriptorSets, 0, this->cameraUniformBuffer.bufferInfos );
    this->rtSceneDescriptors.bindings.write( this->rtSceneDescriptorSets, 1, &directionalLightsInfo );
    this->rtSceneDescriptors.bindings.write( this->rtSceneDescriptorSets, 2, &rtInstancesInfo );
    this->rtSceneDescriptors.bindings.writeArray( this->rtSceneDescriptorSets, 3, this->meshDataBufferInfos.data( ) );

    this->rtSceneDescriptors.bindings.update( );

    // Update RS scene descriptor sets.
    this->rsSceneDescriptors.bindings.write( this->rsSceneDescriptorSets, 0, this->cameraUniformBuffer.bufferInfos );
    this->rsSceneDescriptors.bindings.write( this->rsSceneDescriptorSets, 1, &directionalLightsInfo );
    this->rsSceneDescriptors.bindings.write( this->rsSceneDescriptorSets, 2, &rtInstancesInfo );
    this->rsSceneDescriptors.bindings.update( );
  }

  void Api::updateRayTracingModelData( )
  {
    // Update RT model data.
    for ( const auto& buffer : this->vertexBuffers )
    {
      vk::DescriptorBufferInfo vertexDataBufferInfo( buffer.get( ),
                                                     0,
                                                     VK_WHOLE_SIZE );

      this->vertexDataBufferInfos.push_back( vertexDataBufferInfo );
    }

    for ( const auto& buffer : this->indexBuffers )
    {
      vk::DescriptorBufferInfo indexDataBufferInfo( buffer.get( ),
                                                    0,
                                                    VK_WHOLE_SIZE );

      this->indexDataBufferInfos.push_back( indexDataBufferInfo );
    }

    this->vertexDataDescriptors.bindings.writeArray( this->vertexDataDescriptorSets, 0, this->vertexDataBufferInfos.data( ) );
    this->vertexDataDescriptors.bindings.update( );

    this->indexDataDescriptors.bindings.writeArray( this->indexDataDescriptorSets, 0, this->indexDataBufferInfos.data( ) );
    this->indexDataDescriptors.bindings.update( );
  }

  std::shared_ptr<Geometry> Api::findGeometry( uint32_t geometryIndex )
  {
    for ( std::shared_ptr<Geometry> geometry : this->scene.geometries )
    {
      if ( geometry->geometryIndex == geometryIndex )
      {
        return geometry;
      }
    }

    RX_FATAL( "Could not find geometry in scene." );
    return nullptr;
  }
} // namespace RAYEXEC_NAMESPACE
