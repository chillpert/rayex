#include "api/Api.hpp"

#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Util.hpp"

namespace RAYEX_NAMESPACE
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
  std::vector<PointLightSSBO> dereferencedPointLights;

  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  Api::Api( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera ) :
    _window( std::move( window ) ),
    _camera( std::move( camera ) ),
    _gui( nullptr )
  {
  }

  Api::~Api( )
  {
    components::device.waitIdle( );

    // Gui needs to be destroyed manually, as RAII destruction will not be possible.
    if ( _gui != nullptr )
    {
      _gui->destroy( );
    }
  }

  void Api::setGui( const std::shared_ptr<Gui>& gui, bool initialize )
  {
    if ( _gui != nullptr )
    {
      recreateSwapchain( );
      _gui->destroy( );
    }

    _gui = gui;

    if ( initialize )
    {
      initGui( );
    }
  }

  void Api::initBase( )
  {
    RX_LOG_TIME_START( "Initializing Vulkan (base) ..." );

    // Instance
    _instance = vk::Initializer::initInstance( layers, extensions );

    // Debug messenger
    _debugMessenger.init( );

    // Surface
    _surface.init( );

    // Physical device
    _physicalDevice = vk::Initializer::initPhysicalDevice( );

    // Reassess the support of the preferred surface settings.
    _surface.assessSettings( );

    // Queues
    vk::Initializer::initQueueFamilyIndices( );

    // Logical device
    _device = vk::Initializer::initDevice( deviceExtensions );

    // Retrieve all queue handles.
    components::device.getQueue( components::graphicsFamilyIndex, 0, &components::graphicsQueue );
    components::device.getQueue( components::transferFamilyIndex, 0, &components::transferQueue );

    // Render pass
    initRenderPass( );

    // Swapchain
    _swapchain.init( &_surface, _renderPass.get( ) );
    _settings->_refreshSwapchain = false;

    // Command pools
    _graphicsCmdPool            = vk::Initializer::initCommandPoolUnique( components::graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
    components::graphicsCmdPool = _graphicsCmdPool.get( );

    _transferCmdPool            = vk::Initializer::initCommandPoolUnique( components::transferFamilyIndex, { } );
    components::transferCmdPool = _transferCmdPool.get( );

    // GUI
    initGui( );

    // Create fences and semaphores.
    initSyncObjects( );

    // At this point nodes are unknown. So I just create a storage buffer with the maximum value or with the anticipated value in settings.
    // enables the user to add more nodes later on during runtime.
    uint32_t maxInstanceCount = _settings->_maxGeometryInstances.has_value( ) ? _settings->_maxGeometryInstances.value( ) : components::maxGeometryInstances;
    std::vector<GeometryInstance> geometryInstances( maxInstanceCount );
    _geometryInstancesBuffer.init<GeometryInstance>( geometryInstances );

    uint32_t maxDirectionalLightCount = _settings->_maxDirectionalLights.has_value( ) ? _settings->_maxDirectionalLights.value( ) : components::maxDirectionalLights;
    std::vector<DirectionalLightSSBO> directionalLights( maxDirectionalLightCount );
    _directionalLightsBuffer.init<DirectionalLightSSBO>( directionalLights );

    uint32_t maxPointLightCount = _settings->_maxPointLights.has_value( ) ? _settings->_maxPointLights.value( ) : components::maxPointLights;
    std::vector<PointLightSSBO> pointLights( maxPointLightCount );
    _pointLightsBuffer.init<PointLightSSBO>( pointLights );

    RX_LOG_TIME_STOP( "Finished initializing Vulkan (base)" );
  }

  void Api::initScene( )
  {
    RX_LOG_TIME_START( "Initializing Vulkan (scene) ..." );

    // Uniform buffers for camera
    _cameraUniformBuffer.init<CameraUbo>( );

    _vertexBuffers.resize( components::maxGeometries );
    _indexBuffers.resize( components::maxGeometries );
    _meshBuffers.resize( components::maxGeometries );

    for ( size_t i = 0; i < _scene->_geometries.size( ); ++i )
    {
      if ( _scene->_geometries[i] != nullptr )
      {
        if ( !_scene->_geometries[i]->initialized )
        {
          _vertexBuffers[i].init( _scene->_geometries[i]->vertices );
          _indexBuffers[i].init( _scene->_geometries[i]->indices );
          _meshBuffers[i].init( _scene->_geometries[i]->meshes );
          _scene->_geometries[i]->initialized = true;
        }
      }
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
    _rayTracingBuilder.init( );
    _rayTracingBuilder.createStorageImage( _swapchain.getExtent( ) );
    _rayTracingBuilder.createShaderBindingTable( _rtPipeline.get( ) );

    _settings->_maxRecursionDepth = _rayTracingBuilder.getRtProperties( ).maxRecursionDepth;

    updateAccelerationStructures( );

    // Init and record swapchain command buffers.
    //_swapchainCommandBuffers.reset( );
    _swapchainCommandBuffers.init( _graphicsCmdPool.get( ), components::swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
    recordSwapchainCommandBuffers( );

    RX_LOG_TIME_STOP( "Finished initializing Vulkan (scene)" );
  }

  void Api::update( )
  {
    updateSettings( );

    updateUniformBuffers( );

    // Init geometry storage buffers.
    if ( _scene->_uploadGeometries )
    {
      _scene->_uploadGeometries = false;
    }

    if ( _scene->_uploadGeometryInstancesToBuffer )
    {
      _scene->_uploadGeometryInstancesToBuffer = false;

      if ( !_scene->_geometryInstances.empty( ) )
      {
        // Dereference pointers and store values in new vector that will be uploaded.
        dereferencedGeometryInstances.resize( _scene->_geometryInstances.size( ) );
        std::transform( _scene->_geometryInstances.begin( ),
                        _scene->_geometryInstances.end( ),
                        dereferencedGeometryInstances.begin( ),
                        []( std::shared_ptr<GeometryInstance> instance ) { return *instance; } );

        _geometryInstancesBuffer.fill<GeometryInstance>( dereferencedGeometryInstances.data( ) );
        updateAccelerationStructures( );
      }
    }

    if ( _scene->_uploadDirectionalLightsToBuffer )
    {
      _scene->_uploadDirectionalLightsToBuffer = false;

      if ( !_scene->_directionalLights.empty( ) )
      {
        dereferencedDirectionalLights.resize( _scene->_directionalLights.size( ) );
        std::transform( _scene->_directionalLights.begin( ),
                        _scene->_directionalLights.end( ),
                        dereferencedDirectionalLights.begin( ),
                        []( std::shared_ptr<DirectionalLight> light ) { return DirectionalLightSSBO { glm::vec4( light->ambient, light->ambientIntensity ),
                                                                                                      glm::vec4( light->diffuse, light->diffuseIntensity ),
                                                                                                      glm::vec4( light->specular, light->specularIntensity ),
                                                                                                      glm::vec4( light->direction, 1.0F ) }; } );

        _directionalLightsBuffer.fill<DirectionalLightSSBO>( dereferencedDirectionalLights.data( ) );
      }
    }

    if ( _scene->_uploadPointLightsToBuffer )
    {
      _scene->_uploadPointLightsToBuffer = false;

      if ( !_scene->_pointLights.empty( ) )
      {
        dereferencedPointLights.resize( _scene->_pointLights.size( ) );
        std::transform( _scene->_pointLights.begin( ),
                        _scene->_pointLights.end( ),
                        dereferencedPointLights.begin( ),
                        []( std::shared_ptr<PointLight> light ) { return PointLightSSBO { glm::vec4( light->ambient, light->ambientIntensity ),
                                                                                          glm::vec4( light->diffuse, light->diffuseIntensity ),
                                                                                          glm::vec4( light->specular, light->specularIntensity ),
                                                                                          glm::vec4( light->position, 1.0F ) }; } );

        _pointLightsBuffer.fill<PointLightSSBO>( dereferencedPointLights.data( ) );
      }
    }

    if ( _settings->getJitterCamEnabled( ) )
    {
      if ( components::frameCount >= _settings->_jitterCamSampleRate )
      {
        return;
      }

      // Increment frame counter for jitter cam.
      ++components::frameCount;
    }
  }

  auto Api::prepareFrame( ) -> bool
  {
    // If the window is minimized then simply do not render anything anymore.
    if ( _window->minimized( ) )
    {
      return true;
    }

    // If the window size has changed the swapchain has to be recreated.
    if ( _window->changed( ) || _needSwapchainRecreate )
    {
      _camera->_updateProj = true;

      _needSwapchainRecreate = false;
      recreateSwapchain( );
      return true;
    }

    _swapchain.acquireNextImage( _imageAvailableSemaphores[currentFrame].get( ), nullptr );

    // Wait for the current frame's fences.
    vk::Result result = components::device.waitForFences( 1, &_inFlightFences[currentFrame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

    return false;
  }

  auto Api::submitFrame( ) -> bool
  {
    uint32_t imageIndex = _swapchain.getCurrentImageIndex( );

    // Check if a previous frame is using the current image.
    if ( _imagesInFlight[imageIndex] )
    {
      vk::Result result = components::device.waitForFences( 1, &_imagesInFlight[currentFrame], VK_TRUE, UINT64_MAX );
      RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );
    }

    // This will mark the current image to be in use by this frame.
    _imagesInFlight[imageIndex] = _inFlightFences[currentFrame].get( );

    // Add GUI command buffer to swapchain command buffer if GUI is enabled.
    std::vector<vk::CommandBuffer> commandBuffers = { _swapchainCommandBuffers.get( )[imageIndex] };
    if ( _gui != nullptr )
    {
      commandBuffers.push_back( _gui->getCommandBuffer( imageIndex ) );
    }

    // Reset the signaled state of the current frame's fence to the unsignaled one.
    components::device.resetFences( 1, &_inFlightFences[currentFrame].get( ) );

    // Submits / executes the current image's / framebuffer's command buffer.
    auto pWaitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    auto submitInfo        = vk::Helper::getSubmitInfo( _imageAvailableSemaphores[currentFrame].get( ), _finishedRenderSemaphores[currentFrame].get( ), commandBuffers, pWaitDstStageMask );
    components::graphicsQueue.submit( submitInfo, _inFlightFences[currentFrame].get( ) );

    // Tell the presentation engine that the current image is ready.
    auto presentInfo = vk::Helper::getPresentInfoKHR( _finishedRenderSemaphores[currentFrame].get( ), imageIndex );

    try
    {
      vk::Result result = components::graphicsQueue.presentKHR( presentInfo );
      if ( result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR )
      {
        _needSwapchainRecreate = true;
        RX_WARN( "Swapchain out of data or suboptimal." );
      }
    }
    catch ( ... )
    {
      _needSwapchainRecreate = true;
    }

    prevFrame    = currentFrame;
    currentFrame = ( currentFrame + 1 ) % maxFramesInFlight;
    return false;
  }

  auto Api::render( ) -> bool
  {
    if ( _gui != nullptr )
    {
      _gui->newFrame( );
      _gui->render( );
      _gui->endRender( );
    }

    if ( prepareFrame( ) )
    {
      return true;
    }

    // Wait for previous frame to finish command buffer execution.
    vk::Result result = components::device.waitForFences( 1, &_inFlightFences[prevFrame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );
    recordSwapchainCommandBuffers( );

    if ( _gui != nullptr )
    {
      _gui->renderDrawData( _swapchain.getCurrentImageIndex( ) );
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

    components::device.waitIdle( );

    // Clean up existing swapchain and dependencies.
    _swapchainCommandBuffers.free( );
    _swapchain.destroy( );

    // Recreating the swapchain.
    _swapchain.init( &_surface, _renderPass.get( ) );

    // Recreate storage image with the new swapchain image size and update the ray tracing descriptor set to use the new storage image view.
    _rayTracingBuilder.createStorageImage( _swapchain.getExtent( ) );

    vk::WriteDescriptorSetAccelerationStructureKHR tlasInfo( 1,
                                                             &_rayTracingBuilder.getTlas( ).as.as );

    vk::DescriptorImageInfo storageImageInfo( nullptr,
                                              _rayTracingBuilder.getStorageImageView( ),
                                              vk::ImageLayout::eGeneral );

    _rtDescriptors.bindings.write( _rtDescriptorSets, 0, &tlasInfo );
    _rtDescriptors.bindings.write( _rtDescriptorSets, 1, &storageImageInfo );
    _rtDescriptors.bindings.update( );

    // Swapchain command buffers
    _swapchainCommandBuffers.init( _graphicsCmdPool.get( ), components::swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
    recordSwapchainCommandBuffers( );

    if ( _gui != nullptr )
    {
      _gui->recreate( _swapchain.getExtent( ), _swapchain.getImageViews( ) );
    }

    // Update the camera screen size to avoid image stretching.
    auto screenSize = _swapchain.getExtent( );
    _camera->setSize( screenSize.width, screenSize.height );

    _settings->_refreshSwapchain = false;

    RX_LOG_TIME_STOP( "Finished re-creating swapchain" );
  }

  void Api::updateAccelerationStructures( )
  {
    RX_LOG_TIME_START( "Updating acceleration structures ..." );

    // @TODO Try to call this as few times as possible.
    _rayTracingBuilder.createBottomLevelAS( _vertexBuffers, _indexBuffers );
    _rayTracingBuilder.createTopLevelAS( _scene->_geometryInstances );

    // Update ray tracing descriptor set.
    vk::WriteDescriptorSetAccelerationStructureKHR tlasInfo( 1,
                                                             &_rayTracingBuilder.getTlas( ).as.as );

    vk::DescriptorImageInfo storageImageInfo( nullptr,
                                              _rayTracingBuilder.getStorageImageView( ),
                                              vk::ImageLayout::eGeneral );

    _rtDescriptors.bindings.write( _rtDescriptorSets, 0, &tlasInfo );
    _rtDescriptors.bindings.write( _rtDescriptorSets, 1, &storageImageInfo );
    _rtDescriptors.bindings.update( );

    RX_LOG_TIME_STOP( "Finished updating acceleration structures" );
  }

  void Api::initPipelines( )
  {
    RX_LOG_TIME_START( "Initializing graphic pipelines ..." );

    // Ray tracing pipeline
    std::vector<vk::DescriptorSetLayout> allRtDescriptorSetLayouts = { _rtDescriptors.layout.get( ),
                                                                       _rtSceneDescriptors.layout.get( ),
                                                                       _vertexDataDescriptors.layout.get( ),
                                                                       _indexDataDescriptors.layout.get( ) };
    _rtPipeline.init( allRtDescriptorSetLayouts, _settings );

    // Rasterization pipeline
    glm::vec2 extent = { static_cast<float>( _swapchain.getExtent( ).width ), static_cast<float>( _swapchain.getExtent( ).height ) };
    _viewport        = vk::Viewport( 0.0F, 0.0F, extent.x, extent.y, 0.0F, 1.0F );
    _scissor         = vk::Rect2D( 0, _swapchain.getExtent( ) );

    std::vector<vk::DescriptorSetLayout> allRsDescriptorSetLayouts = { _rsSceneDescriptors.layout.get( ) };

    _rsPipeline.init( allRsDescriptorSetLayouts, _renderPass.get( ), _viewport, _scissor, _settings );

    _pipelinesReady             = true;
    _settings->_refreshPipeline = false;

    RX_LOG_TIME_STOP( "Finished graphic pipelines initialization" );
  }

  void Api::initRenderPass( )
  {
    auto colorAttachmentDescription = vk::Helper::getAttachmentDescription( _surface.getFormat( ) );

    vk::AttachmentReference colorAttachmentReference( 0,                                          // attachment
                                                      vk::ImageLayout::eColorAttachmentOptimal ); // layout

    auto depthAttachmentDescription = vk::Helper::getDepthAttachmentDescription( getSupportedDepthFormat( components::physicalDevice ) );

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

    _renderPass.init( { colorAttachmentDescription, depthAttachmentDescription }, { subpassDescription }, subpassDependencies );
  }

  void Api::initGui( )
  {
    if ( _gui != nullptr )
    {
      initRenderPass( );
      _gui->init( &_surface, _swapchain.getExtent( ), _swapchain.getImageViews( ) );
    }
  }

  void Api::recordSwapchainCommandBuffers( )
  {
    RX_ASSERT( _pipelinesReady, "Can not record swapchain command buffers because the pipelines have not been initialized yet." );

    if ( _settings->_rayTrace )
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

    for ( auto geometry : _scene->_geometries )
    {
      temp.emplace( geometry->path, 0 );
    }

    for ( const auto& geometryInstance : _scene->_geometryInstances )
    {
      std::shared_ptr<Geometry> it = findGeometry( geometryInstance->geometryIndex );
      RX_ASSERT( it != nullptr, "Could not find model. Did you forget to introduce the renderer to this model using Rayex::setModels( ) after initializing the renderer?" );

      auto it2 = temp.find( it->path );
      if ( it2 != temp.end( ) )
      {
        ++( it2->second );
      }
    }

    // Set up render pass begin info
    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color        = { Util::vec4toArray( _settings->getClearColor( ) ) };
    clearValues[1].depthStencil = vk::ClearDepthStencilValue { 1.0F, 0 };

    // Start recording the swapchain framebuffers
    for ( size_t imageIndex = 0; imageIndex < _swapchainCommandBuffers.get( ).size( ); ++imageIndex )
    {
      _swapchainCommandBuffers.begin( imageIndex );

      _renderPass.begin( _swapchain.getFramebuffer( imageIndex ),
                         _swapchainCommandBuffers.get( imageIndex ),
                         { 0, _swapchain.getExtent( ) },
                         { clearValues[0], clearValues[1] } );

      _swapchainCommandBuffers.get( imageIndex ).bindPipeline( vk::PipelineBindPoint::eGraphics, _rsPipeline.get( ) ); // CMD

      // Dynamic states
      vk::Viewport viewport = _viewport;
      viewport.width        = static_cast<float>( _window->getWidth( ) );
      viewport.height       = static_cast<float>( _window->getHeight( ) );

      _swapchainCommandBuffers.get( imageIndex ).setViewport( 0, 1, &viewport ); // CMD

      vk::Rect2D scissor = _scissor;
      scissor.extent     = _window->getExtent( );

      _swapchainCommandBuffers.get( imageIndex ).setScissor( 0, 1, &scissor ); // CMD

      // Draw models
      uint32_t id = 0;
      for ( const auto& geometryInstance : _scene->_geometryInstances )
      {
        auto geo = findGeometry( geometryInstance->geometryIndex );
        RX_ASSERT( geo != nullptr, "Could not find model. Did you forget to introduce the renderer to this model using Rayex::setModels( ) after initializing the renderer?" );

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

        _swapchainCommandBuffers.get( imageIndex ).pushConstants( _rsPipeline.getLayout( ),         // layout
                                                                  vk::ShaderStageFlagBits::eVertex, // stageFlags
                                                                  0,                                // offset
                                                                  sizeof( uint32_t ),               // size
                                                                  &id );                            // pValues

        std::array<vk::Buffer, 1> vertexBuffers { _vertexBuffers[geometryInstance->geometryIndex].get( ) };
        std::array<vk::DeviceSize, 1> offsets { 0 };

        _swapchainCommandBuffers.get( imageIndex ).bindVertexBuffers( 0,                     // first binding
                                                                      1,                     // binding count
                                                                      vertexBuffers.data( ), // pBuffers
                                                                      offsets.data( ) );     // pOffsets

        _swapchainCommandBuffers.get( imageIndex ).bindIndexBuffer( _indexBuffers[geometryInstance->geometryIndex].get( ),
                                                                    0, // offset
                                                                    vk::IndexType::eUint32 );

        std::vector<vk::DescriptorSet> descriptorSets = { _rsSceneDescriptorSets[imageIndex] };

        _swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _rsPipeline.getLayout( ),
                                                                       0,                                               // first set
                                                                       static_cast<uint32_t>( descriptorSets.size( ) ), // descriptor set count
                                                                       descriptorSets.data( ),                          // descriptor sets
                                                                       0,                                               // dynamic offset count
                                                                       nullptr );                                       // dynamic offsets

        _swapchainCommandBuffers.get( imageIndex ).drawIndexed( _indexBuffers[geometryInstance->geometryIndex].getCount( ), // index count
                                                                instanceCount,                                              // instance count
                                                                0,                                                          // first index
                                                                0,                                                          // vertex offset
                                                                0 );                                                        // first instance

        ++id;
      }

      _renderPass.end( _swapchainCommandBuffers.get( imageIndex ) );
      _swapchainCommandBuffers.end( imageIndex );
    }
  }

  void Api::rayTrace( )
  {
    auto directionalLightCount = static_cast<uint32_t>( _scene->_directionalLights.size( ) );
    auto pointLightCount       = static_cast<uint32_t>( _scene->_pointLights.size( ) );

    // Start recording the swapchain framebuffers?
    for ( size_t imageIndex = 0; imageIndex < _swapchainCommandBuffers.get( ).size( ); ++imageIndex )
    {
      _swapchainCommandBuffers.begin( imageIndex );

      RayTracePushConstants chitPc = { _settings->getClearColor( ),
                                       components::frameCount,
                                       _settings->getJitterCamSampleRatePerRayGen( ),
                                       _settings->getSsaaSampleRate( ),
                                       static_cast<uint32_t>( _settings->getJitterCamEnabled( ) ),
                                       static_cast<uint32_t>( _settings->getSsaaEnabled( ) ),
                                       directionalLightCount,
                                       pointLightCount };

      _swapchainCommandBuffers.get( imageIndex ).pushConstants( _rtPipeline.getLayout( ),                                                                                          // layout
                                                                vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eMissKHR | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                                                0,                                                                                                                 // offset
                                                                sizeof( RayTracePushConstants ),                                                                                   // size
                                                                &chitPc );                                                                                                         // pValues

      _swapchainCommandBuffers.get( imageIndex ).bindPipeline( vk::PipelineBindPoint::eRayTracingKHR, _rtPipeline.get( ) );

      std::vector<vk::DescriptorSet> descriptorSets = { _rtDescriptorSets[imageIndex],
                                                        _rtSceneDescriptorSets[imageIndex],
                                                        _vertexDataDescriptorSets[imageIndex],
                                                        _indexDataDescriptorSets[imageIndex] };

      _swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eRayTracingKHR, _rtPipeline.getLayout( ),
                                                                     0,                                               // first set
                                                                     static_cast<uint32_t>( descriptorSets.size( ) ), // descriptor set count
                                                                     descriptorSets.data( ),                          // descriptor sets
                                                                     0,                                               // dynamic offset count
                                                                     nullptr );                                       // dynamic offsets

      _rayTracingBuilder.rayTrace( _swapchainCommandBuffers.get( imageIndex ), _swapchain.getImage( imageIndex ), _swapchain.getExtent( ) );

      _swapchainCommandBuffers.end( imageIndex );
    }
  }

  void Api::initSyncObjects( )
  {
    _imageAvailableSemaphores.resize( maxFramesInFlight );
    _finishedRenderSemaphores.resize( maxFramesInFlight );
    _inFlightFences.resize( maxFramesInFlight );
    _imagesInFlight.resize( components::swapchainImageCount, nullptr );

    for ( size_t i = 0; i < maxFramesInFlight; ++i )
    {
      _imageAvailableSemaphores[i] = vk::Initializer::initSemaphoreUnique( );
      _finishedRenderSemaphores[i] = vk::Initializer::initSemaphoreUnique( );
      _inFlightFences[i]           = vk::Initializer::initFenceUnique( vk::FenceCreateFlagBits::eSignaled );
    }
  }

  void Api::initDescriptorSets( )
  {
    // Create the ray tracing descriptor set layout
    {
      // TLAS
      _rtDescriptors.bindings.add( 0, vk::DescriptorType::eAccelerationStructureKHR, vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR, 1, vk::DescriptorBindingFlagBits::ePartiallyBound );
      // Output image
      _rtDescriptors.bindings.add( 1, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eRaygenKHR, 1, vk::DescriptorBindingFlagBits::ePartiallyBound );

      _rtDescriptors.layout = _rtDescriptors.bindings.initLayoutUnique( );
      _rtDescriptors.pool   = _rtDescriptors.bindings.initPoolUnique( components::swapchainImageCount );
      _rtDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _rtDescriptors.pool, _rtDescriptors.layout );
    }

    // RT Scene descriptor set layout.
    {
      // Camera uniform buffer
      _rtSceneDescriptors.bindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eRaygenKHR );
      // Directional lights storage buffer
      _rtSceneDescriptors.bindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Point lights storage buffer
      _rtSceneDescriptors.bindings.add( 2, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Scene description buffer
      _rtSceneDescriptors.bindings.add( 3, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );
      // Materials
      _rtSceneDescriptors.bindings.add( 4, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, components::maxGeometries, vk::DescriptorBindingFlagBits::eVariableDescriptorCount );

      _rtSceneDescriptors.layout = _rtSceneDescriptors.bindings.initLayoutUnique( );
      _rtSceneDescriptors.pool   = _rtSceneDescriptors.bindings.initPoolUnique( static_cast<uint32_t>( components::maxGeometryInstances ) * components::swapchainImageCount, vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind );
      _rtSceneDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _rtSceneDescriptors.pool, _rtSceneDescriptors.layout );
    }

    // RS Scene descriptor set layout.
    {
      // Camera uniform buffer
      _rsSceneDescriptors.bindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex );
      // Lights storage buffer
      _rsSceneDescriptors.bindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment );
      // Scene description buffer
      _rsSceneDescriptors.bindings.add( 2, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eVertex );

      _rsSceneDescriptors.layout = _rsSceneDescriptors.bindings.initLayoutUnique( );
      _rsSceneDescriptors.pool   = _rsSceneDescriptors.bindings.initPoolUnique( components::swapchainImageCount );
      _rsSceneDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _rsSceneDescriptors.pool, _rsSceneDescriptors.layout );
    }

    // Vertex model data descriptor set layout.
    {
      _vertexDataDescriptors.bindings.add( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, components::maxGeometries, vk::DescriptorBindingFlagBits::eVariableDescriptorCount );

      _vertexDataDescriptors.layout = _vertexDataDescriptors.bindings.initLayoutUnique( );
      _vertexDataDescriptors.pool   = _vertexDataDescriptors.bindings.initPoolUnique( static_cast<uint32_t>( components::maxGeometryInstances ) * components::swapchainImageCount, vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind );
      _vertexDataDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _vertexDataDescriptors.pool, _vertexDataDescriptors.layout );
    }

    // Index model data descriptor set layout.
    {
      _indexDataDescriptors.bindings.add( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR, components::maxGeometries, vk::DescriptorBindingFlagBits::eVariableDescriptorCount );

      _indexDataDescriptors.layout = _indexDataDescriptors.bindings.initLayoutUnique( );
      _indexDataDescriptors.pool   = _indexDataDescriptors.bindings.initPoolUnique( static_cast<uint32_t>( components::maxGeometryInstances ) * components::swapchainImageCount, vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind );
      _indexDataDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _indexDataDescriptors.pool, _indexDataDescriptors.layout );
    }
  }

  void Api::updateSettings( )
  {
    if ( _settings->_refreshPipeline )
    {
      _settings->_refreshPipeline = false;

      components::device.waitIdle( );

#ifdef RX_COPY_ASSETS
      // Copies shader resources to binary output directory. This way a shader can be changed during runtime.
      // Make sure only to edit the ones in /assets/shaders and not in /build/bin/debug/assets/shaders as the latter gets overridden.
      RX_INFO( "Copying shader resources to binary output directory. " );
      std::filesystem::copy( RX_ASSETS_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
#endif

      initPipelines( );
      _rayTracingBuilder.createShaderBindingTable( _rtPipeline.get( ) );
    }

    if ( _settings->_refreshSwapchain )
    {
      _settings->_refreshSwapchain = false;

      recreateSwapchain( );
    }
  }

  void Api::updateUniformBuffers( )
  {
    uint32_t imageIndex = _swapchain.getCurrentImageIndex( );

    // Upload camera.
    if ( _camera != nullptr )
    {
      if ( _camera->_updateView )
      {
        _cameraUbo.view        = _camera->getViewMatrix( );
        _cameraUbo.viewInverse = _camera->getViewInverseMatrix( );

        _camera->_updateView = false;
      }

      if ( _camera->_updateProj )
      {
        _cameraUbo.projection        = _camera->getProjectionMatrix( );
        _cameraUbo.projectionInverse = _camera->getProjectionInverseMatrix( );

        _camera->_updateProj = false;
      }

      _cameraUbo.position = glm::vec4( _camera->getPosition( ), 1.0F );
    }

    _cameraUniformBuffer.upload<CameraUbo>( imageIndex, _cameraUbo );
  }

  void Api::updateSceneDescriptors( )
  {
    vk::DescriptorBufferInfo rtInstancesInfo( _geometryInstancesBuffer.get( ),
                                              0,
                                              VK_WHOLE_SIZE );

    vk::DescriptorBufferInfo directionalLightsInfo( _directionalLightsBuffer.get( ),
                                                    0,
                                                    VK_WHOLE_SIZE );

    vk::DescriptorBufferInfo pointLightsInfo( _pointLightsBuffer.get( ),
                                              0,
                                              VK_WHOLE_SIZE );

    RX_ASSERT( _meshBuffers.size( ) <= components::maxGeometries, "Can not bind more than ", components::maxGeometries, " meshes." );

    for ( size_t i = 0; i < components::maxGeometries; ++i )
    {
      if ( i < _meshBuffers.size( ) )
      {
        vk::DescriptorBufferInfo meshDataBufferInfo( _meshBuffers[i].get( ),
                                                     0,
                                                     VK_WHOLE_SIZE );

        _meshDataBufferInfos.push_back( meshDataBufferInfo );
      }
      else
      {
        vk::DescriptorBufferInfo meshDataBufferInfo( nullptr,
                                                     0,
                                                     VK_WHOLE_SIZE );

        _meshDataBufferInfos.push_back( meshDataBufferInfo );
      }
    }

    // Update RT scene descriptor sets.
    _rtSceneDescriptors.bindings.write( _rtSceneDescriptorSets, 0, _cameraUniformBuffer._bufferInfos );
    _rtSceneDescriptors.bindings.write( _rtSceneDescriptorSets, 1, &directionalLightsInfo );
    _rtSceneDescriptors.bindings.write( _rtSceneDescriptorSets, 2, &pointLightsInfo );
    _rtSceneDescriptors.bindings.write( _rtSceneDescriptorSets, 3, &rtInstancesInfo );
    _rtSceneDescriptors.bindings.writeArray( _rtSceneDescriptorSets, 4, _meshDataBufferInfos.data( ) );

    _rtSceneDescriptors.bindings.update( );

    // Update RS scene descriptor sets.
    _rsSceneDescriptors.bindings.write( _rsSceneDescriptorSets, 0, _cameraUniformBuffer._bufferInfos );
    _rsSceneDescriptors.bindings.write( _rsSceneDescriptorSets, 1, &directionalLightsInfo );
    _rsSceneDescriptors.bindings.write( _rsSceneDescriptorSets, 2, &rtInstancesInfo );
    _rsSceneDescriptors.bindings.update( );
  }

  void Api::updateRayTracingModelData( )
  {
    RX_ASSERT( _scene->_geometries.size( ) <= components::maxGeometries, "Can not bind more than ", components::maxGeometries, " geometries." );

    // Update RT model data.
    for ( size_t i = 0; i < components::maxGeometries; ++i )
    {
      if ( i < _vertexBuffers.size( ) )
      {
        vk::DescriptorBufferInfo vertexDataBufferInfo( _vertexBuffers[i].get( ),
                                                       0,
                                                       VK_WHOLE_SIZE );

        _vertexDataBufferInfos.push_back( vertexDataBufferInfo );

        vk::DescriptorBufferInfo indexDataBufferInfo( _indexBuffers[i].get( ),
                                                      0,
                                                      VK_WHOLE_SIZE );

        _indexDataBufferInfos.push_back( indexDataBufferInfo );
      }
      else
      {
        vk::DescriptorBufferInfo bufferInfo( nullptr,
                                             0,
                                             VK_WHOLE_SIZE );

        _vertexDataBufferInfos.push_back( bufferInfo );
        _indexDataBufferInfos.push_back( bufferInfo );
      }
    }

    _vertexDataDescriptors.bindings.writeArray( _vertexDataDescriptorSets, 0, _vertexDataBufferInfos.data( ) );
    _vertexDataDescriptors.bindings.update( );

    _indexDataDescriptors.bindings.writeArray( _indexDataDescriptorSets, 0, _indexDataBufferInfos.data( ) );
    _indexDataDescriptors.bindings.update( );
  }

  auto Api::findGeometry( uint32_t geometryIndex ) -> std::shared_ptr<Geometry>
  {
    for ( std::shared_ptr<Geometry> geometry : _scene->_geometries )
    {
      if ( geometry->geometryIndex == geometryIndex )
      {
        return geometry;
      }
    }

    RX_FATAL( "Could not find geometry in scene." );
    return nullptr;
  }
} // namespace RAYEX_NAMESPACE
