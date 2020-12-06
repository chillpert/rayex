#include "api/Api.hpp"

#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Util.hpp"

namespace RAYEX_NAMESPACE
{
  const std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };
#ifdef RX_DEBUG
  std::vector<const char*> extensions = { "VK_EXT_debug_utils", "VK_KHR_get_physical_device_properties2" };
#else
  std::vector<const char*> extensions;
#endif

  std::vector<const char*> deviceExtensions = {
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
    VK_KHR_RAY_TRACING_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SHADER_CLOCK_EXTENSION_NAME
  };

  size_t currentFrame = 0;
  size_t prevFrame    = 0;

  std::vector<GeometryInstanceSSBO> memAlignedGeometryInstances;
  std::vector<MeshSSBO> memAlignedMeshes;

  CameraUbo cameraUbo;

  std::shared_ptr<Geometry> triangle                 = nullptr; ///< A dummy triangle that will be placed in the scene if it empty. This assures the AS creation.
  std::shared_ptr<GeometryInstance> triangleInstance = nullptr;

  bool removeEnvironmentMap = false;

  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

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

    // Retrieve and add window extensions to other extensions.
    auto windowExtensions = _window->getExtensions( );
    extensions.insert( extensions.end( ), windowExtensions.begin( ), windowExtensions.end( ) );

    // Instance
    _instance = vk::Initializer::initInstance( layers, extensions );

    // Debug messenger
    _debugMessenger.init( );

    // Surface
    _surface.init( _window );

    // Physical device
    components::physicalDevice = vk::Initializer::initPhysicalDevice( );

    // Reassess the support of the preferred surface settings.
    _surface.assessSettings( );

    // Queues
    vk::Initializer::initQueueFamilyIndices( );

    // Logical device
    _device = vk::Initializer::initDevice( deviceExtensions );

    // Retrieve all queue handles.
    components::device.getQueue( components::graphicsFamilyIndex, 0, &components::graphicsQueue );
    components::device.getQueue( components::transferFamilyIndex, 0, &components::transferQueue );

    // Command pools
    _graphicsCmdPool            = vk::Initializer::initCommandPoolUnique( components::graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
    components::graphicsCmdPool = _graphicsCmdPool.get( );

    _transferCmdPool            = vk::Initializer::initCommandPoolUnique( components::transferFamilyIndex, { } );
    components::transferCmdPool = _transferCmdPool.get( );

    // Post processing renderer
    _postProcessingRenderer.initColorImage( _surface.getExtent( ) );
    _postProcessingRenderer.initDepthImage( _surface.getExtent( ) );
    _postProcessingRenderer.initRenderPass( _surface.getFormat( ) );

    // Swapchain
    _swapchain.init( &_surface, _postProcessingRenderer.getRenderPass( ).get( ) );
    _settings._refreshSwapchain = false;

    // GUI
    initGui( );

    // Create fences and semaphores.
    initSyncObjects( );

    // Path tracer (part 1)
    _pathTracer.init( );
    _settings._maxRecursionDepth = _pathTracer.getDevicePathTracingProperties( ).maxRecursionDepth;

    // Resize and initialize buffers with "dummy data".
    // The advantage of doing this is that the buffers are all initialized right away (even though it is invalid data) and
    // this makes it possible to call fill instead of init again, when changing any of the data below.
    std::vector<GeometryInstanceSSBO> geometryInstances( _settings._maxGeometryInstances );
    _geometryInstancesBuffer.init( geometryInstances, components::maxResources );

    _vertexBuffers.resize( static_cast<size_t>( _settings._maxGeometry ) );
    _indexBuffers.resize( static_cast<size_t>( _settings._maxGeometry ) );
    _meshBuffers.resize( static_cast<size_t>( _settings._maxMeshes ) );
    _textures.resize( static_cast<size_t>( _settings._maxTextures ) );

    RX_LOG_TIME_STOP( "Finished initializing Vulkan (base)" );
  }

  void Api::initScene( )
  {
    RX_LOG_TIME_START( "Initializing Vulkan (scene) ..." );

    // Uniform buffers for camera
    _cameraUniformBuffer.init<CameraUbo>( );

    // Descriptor sets and layouts
    initDescriptorSets( );

    // Update RT scene descriptor sets.
    updateSceneDescriptors( );

    // Initialize the path tracing pipeline.
    initPipelines( );

    // Path tracer (part 2)
    _pathTracer.createStorageImage( _swapchain.getExtent( ) );
    _pathTracer.createShaderBindingTable( );

    // Post processing renderer
    _postProcessingRenderer.initDescriptorSet( );

    auto extent = _swapchain.getExtent( );
    auto width  = static_cast<float>( extent.width );
    auto height = static_cast<float>( extent.height );

    vk::Viewport viewport( 0.0F, 0.0F, width, height, 0.0F, 1.0F );
    vk::Rect2D scissor( { { 0, 0 }, extent } );

    _postProcessingRenderer.initPipeline( viewport, scissor );
    _postProcessingRenderer.updateDescriptors( _pathTracer.getStorageImageView( ), _pathTracer.getStorageImage( ).getLayout( ) );

    // Init and record swapchain command buffers.
    _swapchainCommandBuffers.init( _graphicsCmdPool.get( ), components::swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );

    // If user has not set an environment map themself set a default one, to guarantee successful start up.
    if ( !_scene._uploadEnvironmentMap )
    {
      _scene.setEnvironmentMap( "" );
      removeEnvironmentMap = true;
    }

    RX_LOG_TIME_STOP( "Finished initializing Vulkan (scene)" );
  }

  void Api::update( )
  {
    uint32_t imageIndex = _swapchain.getCurrentImageIndex( );

    _camera = _scene._currentCamera;

    // Upload camera.
    if ( _camera != nullptr )
    {
      if ( _camera->_updateView )
      {
        cameraUbo.view        = _camera->getViewMatrix( );
        cameraUbo.viewInverse = _camera->getViewInverseMatrix( );

        _camera->_updateView = false;
      }

      if ( _camera->_updateProj )
      {
        cameraUbo.projection        = _camera->getProjectionMatrix( );
        cameraUbo.projectionInverse = _camera->getProjectionInverseMatrix( );

        _camera->_updateProj = false;
      }

      cameraUbo.position = glm::vec4( _camera->getPosition( ), 1.0F );
    }

    _cameraUniformBuffer.upload<CameraUbo>( imageIndex % maxFramesInFlight, cameraUbo );

    // If the scene is empty add a dummy triangle so that a TLAS can be built successfully.
    if ( _scene._geometryInstances.empty( ) )
    {
      _scene._dummy = true;

      vk::Result result = components::device.waitForFences( 1, &_inFlightFences[prevFrame].get( ), VK_TRUE, UINT64_MAX );
      RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

      Vertex v1;
      v1.normal = glm::vec3( 0.0F, 1.0F, 0.0F );
      v1.pos    = glm::vec3( -0.00001F, 0.0F, 0.00001F );

      Vertex v2;
      v2.normal = glm::vec3( 0.0F, 1.0F, 0.0F );
      v2.pos    = glm::vec3( 0.00001F, 0.0F, 0.00001F );

      Vertex v3;
      v3.normal = glm::vec3( 0.0F, 1.0F, 0.0F );
      v3.pos    = glm::vec3( 0.00001F, 0.0F, -0.00001F );

      triangle                = std::make_shared<Geometry>( );
      triangle->vertices      = { v1, v2, v3 };
      triangle->indices       = { 0, 1, 2 };
      triangle->geometryIndex = components::geometryIndex++;
      triangle->meshes.push_back( { { }, 0 } );
      triangle->path = "Custom Dummy Triangle";

      triangleInstance = instance( triangle );

      _scene.submitGeometry( triangle );
      _scene.submitGeometryInstance( triangleInstance );

      RX_VERBOSE( "Scene is empty. Added dummy element." );
    }
    else
    {
      if ( triangle != nullptr && _scene._geometryInstances.size( ) > 1 )
      {
        _scene._dummy = false;

        RX_VERBOSE( "Removing dummy element." );
        _scene.removeGeometry( triangle );
        triangle = nullptr;
      }
    }

    if ( _scene._deleteTextures )
    {
      _scene._deleteTextures = false;

      _textures.clear( );
      _textures.resize( static_cast<size_t>( _settings._maxTextures ) );
    }

    if ( _scene._uploadEnvironmentMap )
    {
      _scene._uploadEnvironmentMap = false;

      vk::Result result = components::device.waitForFences( 1, &_inFlightFences[prevFrame].get( ), VK_TRUE, UINT64_MAX );
      RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

      _environmentMap.init( _scene._environmentMapTexturePath );

      if ( removeEnvironmentMap )
      {
        _scene.removeEnvironmentMap( );
        removeEnvironmentMap = false;
      }

      updateGeometryDescriptors( );
    }

    // Init geometry storage buffers.
    if ( _scene._uploadGeometries )
    {
      _scene._uploadGeometries = false;

      for ( size_t i = 0; i < _scene._geometries.size( ); ++i )
      {
        if ( i < _scene._geometries.size( ) )
        {
          if ( _scene._geometries[i] != nullptr )
          {
            if ( !_scene._geometries[i]->initialized )
            {
              _vertexBuffers[i].init( _scene._geometries[i]->vertices, 1, true );
              _indexBuffers[i].init( _scene._geometries[i]->indices );

              memAlignedMeshes.resize( _scene._geometries[i]->meshes.size( ) );

              // Textures
              int j                 = 0;
              float diffuseTexIndex = -1.0F;
              for ( const auto& mesh : _scene._geometries[i]->meshes )
              {
                diffuseTexIndex = -1.0F;

                // @todo Rare bug where textureIndex would be std::numeric_limits<size_t>::max( ). Do not know how to reproduce yet.
                size_t textureIndex = std::numeric_limits<size_t>::max( );
                for ( size_t i = 0; i < _textures.size( ); ++i )
                {
                  if ( _textures[i] == nullptr )
                  {
                    textureIndex = i;
                  }
                }

                RX_ASSERT( textureIndex != std::numeric_limits<size_t>::max( ), "Can not have more than ", _settings._maxTextures, " textures." );

                if ( _textures[textureIndex] == nullptr && !mesh.material.diffuseTexPath.empty( ) )
                {
                  auto texture = std::make_shared<Texture>( );
                  texture->init( mesh.material.diffuseTexPath );
                  diffuseTexIndex         = static_cast<float>( textureIndex );
                  _textures[textureIndex] = texture;
                }

                memAlignedMeshes[j] = MeshSSBO { glm::vec4( mesh.material.ambient, -1.0F ),
                                                 glm::vec4( mesh.material.diffuse, diffuseTexIndex ),
                                                 glm::vec4( mesh.material.specular, -1.0F ),
                                                 glm::vec4( mesh.material.emission, 1.0F ),
                                                 glm::vec4( mesh.material.transmittance, 1.0F ),
                                                 { },
                                                 mesh.indexOffset };
                ++j;
              }

              // Meshes
              _meshBuffers[i].init( memAlignedMeshes );

              _scene._geometries[i]->initialized = true;
              RX_SUCCESS( "Initialized Geometries." );
            }
          }
        }
      }

      vk::Result result = components::device.waitForFences( 1, &_inFlightFences[prevFrame].get( ), VK_TRUE, UINT64_MAX );
      RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

      updateGeometryDescriptors( ); // Contains descriptors for vertices and indices.

      RX_SUCCESS( "Uploaded Geometries." );
    }

    if ( _scene._uploadGeometryInstancesToBuffer )
    {
      // This check should not exist - @self investigate!
      if ( imageIndex % maxFramesInFlight == 0 )
      {
        _scene._uploadGeometryInstancesToBuffer = false;

        if ( !_scene._geometryInstances.empty( ) )
        {
          memAlignedGeometryInstances.resize( _scene._geometryInstances.size( ) );
          std::transform( _scene._geometryInstances.begin( ), _scene._geometryInstances.end( ), memAlignedGeometryInstances.begin( ),
                          []( std::shared_ptr<GeometryInstance> instance ) { return GeometryInstanceSSBO { instance->transform,
                                                                                                           instance->transformIT,
                                                                                                           instance->geometryIndex }; } );

          _geometryInstancesBuffer.upload( memAlignedGeometryInstances, imageIndex % maxFramesInFlight );

          updateAccelerationStructuresDescriptors( );

          RX_SUCCESS( "Uploaded geometry instances." );
        }
      }
    }
    else
    {
      if ( !_scene._geometryInstances.empty( ) )
      {
        if ( !_pathTracer.instances.empty( ) )
        {
          size_t i = 0;
          for ( BlasInstance& instance : _pathTracer.instances )
          {
            instance.transform = _scene._geometryInstances[i]->transform;
            ++i;
          }

          _pathTracer.buildTlas( _pathTracer.instances, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace | vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate, true );
        }
      }
    }

    // Increment frame counter for jitter cam.
    if ( _settings._accumulateFrames )
    {
      ++components::frameCount;
    }

  } // namespace RAYEX_NAMESPACE

  void Api::prepareFrame( )
  {
    _swapchain.acquireNextImage( _imageAvailableSemaphores[currentFrame].get( ), nullptr );
  }

  void Api::submitFrame( )
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

    std::vector<vk::CommandBuffer> commandBuffers = { _swapchainCommandBuffers.get( )[imageIndex] };

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
  }

  void Api::updateSettings( )
  {
    // Handle pipeline refresh
    if ( _settings._refreshPipeline )
    {
      _settings._refreshPipeline = false;

      // Calling wait idle, because pipeline recreation is assumed to be a very rare event to happen.
      components::device.waitIdle( );

#ifdef RX_COPY_ASSETS
      // Copies shader resources to binary output directory. This way a shader can be changed during runtime.
      // Make sure only to edit the ones in /assets/shaders and not in /build/bin/debug/assets/shaders as the latter gets overridden.
      RX_INFO( "Copying shader resources to binary output directory. " );
      std::filesystem::copy( RX_ASSETS_PATH "shaders", RX_PATH_TO_LIBRARY "shaders", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
#endif

      initPipelines( );
      _pathTracer.createShaderBindingTable( );
    }

    // Handle swapchain refresh
    if ( _settings._refreshSwapchain )
    {
      _settings._refreshSwapchain = false;

      recreateSwapchain( );
    }
  }

  auto Api::render( ) -> bool
  {
    updateSettings( );

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

    prepareFrame( );

    update( );

    recordSwapchainCommandBuffers( );

    submitFrame( );

    return true;
  }

  void Api::recreateSwapchain( )
  {
    RX_LOG_TIME_START( "Re-creating swapchain ..." );

    // Waiting idle because this event is considered to be very rare.
    components::device.waitIdle( );

    // Clean up existing swapchain and dependencies.
    _swapchain.destroy( );

    // Recreating the swapchain.
    _swapchain.init( &_surface, _postProcessingRenderer.getRenderPass( ).get( ) );

    // Recreate storage image with the new swapchain image size and update the path tracing descriptor set to use the new storage image view.
    _pathTracer.createStorageImage( _swapchain.getExtent( ) );

    _postProcessingRenderer.updateDescriptors( _pathTracer.getStorageImageView( ), _pathTracer.getStorageImage( ).getLayout( ) );

    vk::WriteDescriptorSetAccelerationStructureKHR tlasInfo( 1,
                                                             &_pathTracer.getTlas( ).as.as );

    vk::DescriptorImageInfo storageImageInfo( nullptr,
                                              _pathTracer.getStorageImageView( ),
                                              vk::ImageLayout::eGeneral ); // @todo Do not hardcode this value.

    _ptDescriptors.bindings.write( _ptDescriptorSets, 0, &tlasInfo );
    _ptDescriptors.bindings.write( _ptDescriptorSets, 1, &storageImageInfo );
    _ptDescriptors.bindings.update( );

    // Swapchain command buffers
    _swapchainCommandBuffers.init( _graphicsCmdPool.get( ), components::swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );
    recordSwapchainCommandBuffers( );

    if ( _gui != nullptr )
    {
      _gui->recreate( _swapchain.getExtent( ) );
    }

    // Update the camera screen size to avoid image stretching.
    auto screenSize = _swapchain.getExtent( );
    _camera->setSize( screenSize.width, screenSize.height );

    _settings._refreshSwapchain = false;

    RX_LOG_TIME_STOP( "Finished re-creating swapchain" );
  }

  void Api::updateAccelerationStructuresDescriptors( )
  {
    RX_LOG_TIME_START( "Updating acceleration structures ..." );

    // @TODO Try to call this as few times as possible.
    _pathTracer.createBottomLevelAS( _vertexBuffers, _indexBuffers, _scene._geometries );
    _pathTracer.createTopLevelAS( _scene._geometryInstances );

    // Update path tracing descriptor set.
    vk::WriteDescriptorSetAccelerationStructureKHR tlasInfo( 1,
                                                             &_pathTracer.getTlas( ).as.as );

    vk::DescriptorImageInfo storageImageInfo( nullptr,
                                              _pathTracer.getStorageImageView( ),
                                              vk::ImageLayout::eGeneral );

    _ptDescriptors.bindings.write( _ptDescriptorSets, 0, &tlasInfo );
    _ptDescriptors.bindings.write( _ptDescriptorSets, 1, &storageImageInfo );
    _ptDescriptors.bindings.update( );

    RX_LOG_TIME_STOP( "Finished updating acceleration structures" );
  }

  void Api::initPipelines( )
  {
    RX_LOG_TIME_START( "Initializing graphic pipelines ..." );

    // path tracing pipeline
    std::vector<vk::DescriptorSetLayout> allRtDescriptorSetLayouts = { _ptDescriptors.layout.get( ),
                                                                       _ptSceneDescriptors.layout.get( ),
                                                                       _geometryDescriptors.layout.get( ) };

    _pathTracer.createPipeline( allRtDescriptorSetLayouts, &_settings );

    _pipelinesReady            = true;
    _settings._refreshPipeline = false;

    RX_LOG_TIME_STOP( "Finished graphic pipelines initialization" );
  }

  void Api::initGui( )
  {
    if ( _gui != nullptr )
    {
      _gui->init( _window->get( ), &_surface, _swapchain.getExtent( ), _postProcessingRenderer.getRenderPass( ).get( ) );
    }
  }

  void Api::recordSwapchainCommandBuffers( )
  {
    RX_ASSERT( _pipelinesReady, "Can not record swapchain command buffers because the pipelines have not been initialized yet." );

    // Wait for previous frame to finish command buffer execution.
    vk::Result result = components::device.waitForFences( 1, &_inFlightFences[prevFrame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );

    PtPushConstants chitPc = { _settings._clearColor,
                               components::frameCount,
                               _settings._perPixelSampleRate,
                               _settings._recursionDepth,
                               static_cast<uint32_t>( _scene._useEnvironmentMap ) };

    // Start recording the swapchain framebuffers?
    for ( size_t imageIndex = 0; imageIndex < _swapchainCommandBuffers.get( ).size( ); ++imageIndex )
    {
      vk::CommandBuffer cmdBuf = _swapchainCommandBuffers.get( imageIndex );

      _swapchainCommandBuffers.begin( imageIndex );
      {
        cmdBuf.pushConstants( _pathTracer.getPipelineLayout( ),                                                                                  // layout
                              vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eMissKHR | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                              0,                                                                                                                 // offset
                              sizeof( PtPushConstants ),                                                                                         // size
                              &chitPc );                                                                                                         // pValues

        cmdBuf.bindPipeline( vk::PipelineBindPoint::eRayTracingKHR, _pathTracer.getPipeline( ) );

        std::vector<vk::DescriptorSet> descriptorSets = { _ptDescriptorSets[imageIndex % maxFramesInFlight],
                                                          _ptSceneDescriptorSets[imageIndex % maxFramesInFlight],
                                                          _geometryDescriptorSets[imageIndex % maxFramesInFlight] };

        cmdBuf.bindDescriptorSets( vk::PipelineBindPoint::eRayTracingKHR,           // pipelineBindPoint
                                   _pathTracer.getPipelineLayout( ),                // layout
                                   0,                                               // first set
                                   static_cast<uint32_t>( descriptorSets.size( ) ), // descriptor set count
                                   descriptorSets.data( ),                          // descriptor sets
                                   0,                                               // dynamic offset count
                                   nullptr );                                       // dynamic offsets

        // First pass (path tracing)
        _pathTracer.pathTrace( cmdBuf, _swapchain.getImage( imageIndex ), _swapchain.getExtent( ) );

        // Second pass
        _postProcessingRenderer.beginRenderPass( cmdBuf, _swapchain.getFramebuffer( imageIndex ), _swapchain.getExtent( ) );
        {
          // 2. Post processing
          _postProcessingRenderer.render( cmdBuf, _swapchain.getExtent( ), imageIndex % maxFramesInFlight );

          // 3. ImGui
          if ( _gui != nullptr )
          {
            _gui->renderDrawData( cmdBuf );
          }
        }
        _postProcessingRenderer.endRenderPass( cmdBuf );
      }
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
    // Create the path tracing descriptor set layout
    {
      // TLAS
      _ptDescriptors.bindings.add( 0,
                                   vk::DescriptorType::eAccelerationStructureKHR,
                                   vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR,
                                   1,
                                   vk::DescriptorBindingFlagBits::eUpdateUnusedWhilePending | vk::DescriptorBindingFlagBits::ePartiallyBound );
      // Output image
      _ptDescriptors.bindings.add( 1,
                                   vk::DescriptorType::eStorageImage,
                                   vk::ShaderStageFlagBits::eRaygenKHR,
                                   1,
                                   vk::DescriptorBindingFlagBits::eUpdateUnusedWhilePending | vk::DescriptorBindingFlagBits::ePartiallyBound );

      _ptDescriptors.layout = _ptDescriptors.bindings.initLayoutUnique( );
      _ptDescriptors.pool   = _ptDescriptors.bindings.initPoolUnique( components::swapchainImageCount );
      _ptDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _ptDescriptors.pool, _ptDescriptors.layout );
    }

    // Scene descriptor set layout.
    {
      // Camera uniform buffer
      _ptSceneDescriptors.bindings.add( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eRaygenKHR );
      // Scene description buffer
      _ptSceneDescriptors.bindings.add( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eClosestHitKHR );

      _ptSceneDescriptors.layout = _ptSceneDescriptors.bindings.initLayoutUnique( );
      _ptSceneDescriptors.pool   = _ptSceneDescriptors.bindings.initPoolUnique( components::maxResources );
      _ptSceneDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _ptSceneDescriptors.pool, _ptSceneDescriptors.layout );
    }

    // Geometry descriptor set layout.
    {
      // Vertex buffers
      _geometryDescriptors.bindings.add( 0,
                                         vk::DescriptorType::eStorageBuffer,
                                         vk::ShaderStageFlagBits::eClosestHitKHR,
                                         _settings._maxGeometry,
                                         vk::DescriptorBindingFlagBits::eUpdateAfterBind );

      // Index buffers
      _geometryDescriptors.bindings.add( 1,
                                         vk::DescriptorType::eStorageBuffer,
                                         vk::ShaderStageFlagBits::eClosestHitKHR,
                                         _settings._maxGeometry,
                                         vk::DescriptorBindingFlagBits::eUpdateAfterBind );

      // Mesh buffers
      _geometryDescriptors.bindings.add( 2,
                                         vk::DescriptorType::eStorageBuffer,
                                         vk::ShaderStageFlagBits::eClosestHitKHR,
                                         _settings._maxMeshes,
                                         vk::DescriptorBindingFlagBits::eUpdateAfterBind );

      // Environment map
      _geometryDescriptors.bindings.add( 3,
                                         vk::DescriptorType::eCombinedImageSampler,
                                         vk::ShaderStageFlagBits::eMissKHR );

      // Textures
      _immutableSamplers.reserve( _settings._maxTextures );
      for ( uint32_t i = 0; i < _settings._maxTextures; ++i )
      {
        // @todo re-use the same sampler for all of these.
        auto samplerCreateInfo = vk::Helper::getSamplerCreateInfo( );
        _immutableSamplers.push_back( std::move( vk::Initializer::initSamplerUnique( samplerCreateInfo ) ) );
      }

      std::vector<vk::Sampler> immutableSamplers( _immutableSamplers.size( ) );
      std::transform( _immutableSamplers.begin( ),
                      _immutableSamplers.end( ),
                      immutableSamplers.begin( ),
                      []( const vk::UniqueSampler& sampler ) { return vk::Sampler( sampler.get( ) ); } );

      _geometryDescriptors.bindings.add( 4,
                                         vk::DescriptorType::eCombinedImageSampler,
                                         vk::ShaderStageFlagBits::eClosestHitKHR,
                                         _settings._maxTextures,
                                         vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::eVariableDescriptorCount,
                                         immutableSamplers.data( ) );

      _geometryDescriptors.layout = _geometryDescriptors.bindings.initLayoutUnique( vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool );
      _geometryDescriptors.pool   = _geometryDescriptors.bindings.initPoolUnique( components::swapchainImageCount, vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind );
      _geometryDescriptorSets     = vk::Initializer::initDescriptorSetsUnique( _geometryDescriptors.pool, _geometryDescriptors.layout );
    }
  }

  void Api::updateSceneDescriptors( )
  {
    // Update scene descriptor sets
    _ptSceneDescriptors.bindings.writeArray( _ptSceneDescriptorSets, 0, _cameraUniformBuffer._bufferInfos.data( ) );
    _ptSceneDescriptors.bindings.writeArray( _ptSceneDescriptorSets, 1, _geometryInstancesBuffer.getDescriptorInfos( ).data( ) );
    _ptSceneDescriptors.bindings.update( );
  }

  void Api::updateGeometryDescriptors( )
  {
    RX_ASSERT( _scene._geometries.size( ) <= _settings._maxGeometry, "Can not bind more than ", _settings._maxGeometry, " geometries." );
    RX_ASSERT( _meshBuffers.size( ) <= _settings._maxMeshes, "Can not bind more than ", _settings._maxMeshes, " meshes." );

    // Vertex buffers infos
    std::vector<vk::DescriptorBufferInfo> vertexBufferInfos;
    vertexBufferInfos.reserve( _vertexBuffers.size( ) );
    for ( const auto& vertexBuffer : _vertexBuffers )
    {
      vk::Buffer buffer = nullptr;
      if ( !vertexBuffer.get( ).empty( ) )
      {
        buffer = vertexBuffer.get( 0 );
      }

      vk::DescriptorBufferInfo vertexDataBufferInfo( buffer,
                                                     0,
                                                     VK_WHOLE_SIZE );

      vertexBufferInfos.push_back( vertexDataBufferInfo );
    }

    // Index buffers infos
    std::vector<vk::DescriptorBufferInfo> indexBufferInfos;
    indexBufferInfos.reserve( _indexBuffers.size( ) );
    for ( const auto& indexBuffer : _indexBuffers )
    {
      vk::DescriptorBufferInfo indexDataBufferInfo( indexBuffer.get( ),
                                                    0,
                                                    VK_WHOLE_SIZE );

      indexBufferInfos.push_back( indexDataBufferInfo );
    }

    // Mesh buffers info (each geometry stores n materials and offsets into the array of index buffers)
    std::vector<vk::DescriptorBufferInfo> meshBufferInfos;
    meshBufferInfos.reserve( _meshBuffers.size( ) );
    for ( auto& meshBuffer : _meshBuffers )
    {
      if ( meshBuffer.getDescriptorInfos( ).empty( ) )
      {
        vk::DescriptorBufferInfo meshBufferInfo( nullptr,
                                                 0,
                                                 VK_WHOLE_SIZE );

        meshBufferInfos.push_back( meshBufferInfo );
      }
      else
      {
        meshBufferInfos.push_back( meshBuffer.getDescriptorInfos( )[0] );
      }
    }

    // Texture samplers
    std::vector<vk::DescriptorImageInfo> textureInfos;
    textureInfos.reserve( _textures.size( ) );
    for ( size_t i = 0; i < _settings._maxTextures; ++i )
    {
      vk::DescriptorImageInfo textureInfo = { };

      if ( _textures[i] != nullptr )
      {
        textureInfo.imageLayout = _textures[i]->getLayout( );
        textureInfo.imageView   = _textures[i]->getImageView( );
        textureInfo.sampler     = _immutableSamplers[i].get( );
      }
      else
      {
        textureInfo.imageLayout = { };
        textureInfo.sampler     = _immutableSamplers[i].get( );
      }

      textureInfos.push_back( textureInfo );
    }

    // Environment map
    vk::DescriptorImageInfo environmentMapTextureInfo;
    if ( _environmentMap.getImageView( ) && _environmentMap.getSampler( ) )
    {
      environmentMapTextureInfo.imageLayout = _environmentMap.getLayout( );
      environmentMapTextureInfo.imageView   = _environmentMap.getImageView( );
      environmentMapTextureInfo.sampler     = _environmentMap.getSampler( );
    }
    else
    {
      RX_FATAL( "No default environment map provided." );
    }

    // Write to and update descriptor bindings
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 0, vertexBufferInfos.data( ) );
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 1, indexBufferInfos.data( ) );
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 2, meshBufferInfos.data( ) );
    _geometryDescriptors.bindings.write( _geometryDescriptorSets, 3, &environmentMapTextureInfo );
    _geometryDescriptors.bindings.writeArray( _geometryDescriptorSets, 4, textureInfos.data( ) );

    _geometryDescriptors.bindings.update( );
  }
} // namespace RAYEX_NAMESPACE
