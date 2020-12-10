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

  void Api::init( )
  {
    RX_LOG_TIME_START( "API start up ..." );

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
    _postProcessingRenderer.initDepthImage( _surface.getExtent( ) );
    _postProcessingRenderer.initRenderPass( _surface.getFormat( ) );

    // Swapchain
    _swapchain.init( &_surface, _postProcessingRenderer.getRenderPass( ).get( ) );
    _settings._refreshSwapchain = false;

    // GUI
    initGui( );

    // Create fences and semaphores.
    _sync.init( );

    // Path tracer
    _pathTracer.init( );
    _settings._maxRecursionDepth = _pathTracer.getDevicePathTracingProperties( ).maxRecursionDepth;

    _scene.prepareBuffers( );

    // Descriptor sets and layouts
    _pathTracer.initDescriptorSet( );
    _scene.initSceneDescriptorSets( );
    _scene.initGeoemtryDescriptorSets( );

    // Default environment map to assure start up.
    _scene.setEnvironmentMap( "" );
    _scene.uploadEnvironmentMap( );
    _scene.removeEnvironmentMap( );

    // Update scene descriptor sets.
    _scene.updateSceneDescriptors( );

    // Initialize the path tracing pipeline.
    initPipelines( );

    _pathTracer.createStorageImage( _swapchain.getExtent( ) );
    _pathTracer.createShaderBindingTable( );

    // Post processing renderer
    _postProcessingRenderer.initDescriptorSet( );
    _postProcessingRenderer.initPipeline( );
    _postProcessingRenderer.updateDescriptors( _pathTracer.getStorageImageInfo( ) );

    // Init and record swapchain command buffers.
    _swapchainCommandBuffers.init( _graphicsCmdPool.get( ), components::swapchainImageCount, vk::CommandBufferUsageFlagBits::eRenderPassContinue );

    RX_LOG_TIME_STOP( "API finished" );
  }

  void Api::update( )
  {
    uint32_t imageIndex        = _swapchain.getCurrentImageIndex( );
    uint32_t maxFramesInFlight = static_cast<uint32_t>( _sync.getMaxFramesInFlight( ) );

    _scene.uploadCameraBuffer( imageIndex % maxFramesInFlight );

    // If the scene is empty add a dummy triangle so that the acceleration structures can be built successfully.
    if ( _scene._geometryInstances.empty( ) )
    {
      _sync.waitForFrame( prevFrame );
      _scene.addDummy( );
    }
    else
    {
      _sync.waitForFrame( prevFrame );
      _scene.removeDummy( );
    }

    if ( _scene._deleteTextures )
    {
      _scene.clearTextures( );
    }

    if ( _scene._uploadEnvironmentMap )
    {
      _sync.waitForFrame( prevFrame );
      _scene.uploadEnvironmentMap( );
      _scene.updateSceneDescriptors( );
    }

    if ( _scene._uploadGeometries )
    {
      _sync.waitForFrame( prevFrame );
      _scene.uploadGeometries( );
      _scene.updateGeoemtryDescriptors( );
    }

    if ( _scene._uploadGeometryInstancesToBuffer )
    {
      _scene.uploadGeometryInstances( imageIndex % maxFramesInFlight );

      // @TODO Try to call this as few times as possible.
      _pathTracer.createBottomLevelAS( _scene._vertexBuffers, _scene._indexBuffers, _scene._geometries );
      _pathTracer.buildTlas( _scene._geometryInstances );
      _pathTracer.updateDescriptors( );
    }
    else
    {
      _pathTracer.buildTlas( _scene._geometryInstances, vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace | vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate, true );
    }

    // Increment frame counter for jitter cam.
    if ( _settings._accumulateFrames )
    {
      ++components::frameCount;
    }
    else
    {
      components::frameCount = -1;
    }

  } // namespace RAYEX_NAMESPACE

  void Api::prepareFrame( )
  {
    _swapchain.acquireNextImage( _sync.getImageAvailableSemaphore( currentFrame ), nullptr );
  }

  void Api::submitFrame( )
  {
    uint32_t imageIndex = _swapchain.getCurrentImageIndex( );
    size_t imageIndex_t = static_cast<size_t>( imageIndex );

    // Check if a previous frame is using the current image.
    if ( _sync.getImageInFlight( imageIndex ) )
    {
      _sync.waitForFrame( currentFrame );
    }

    // This will mark the current image to be in use by this frame.
    _sync.getImageInFlight( imageIndex_t ) = _sync.getInFlightFence( currentFrame );

    vk::CommandBuffer cmdBuf = _swapchainCommandBuffers.get( imageIndex );

    // Reset the signaled state of the current frame's fence to the unsignaled one.
    auto currentInFlightFence_t = _sync.getInFlightFence( currentFrame );
    components::device.resetFences( 1, &currentInFlightFence_t );

    // Submits / executes the current image's / framebuffer's command buffer.
    vk::PipelineStageFlags pWaitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    auto waitSemaphore    = _sync.getImageAvailableSemaphore( currentFrame );
    auto signaleSemaphore = _sync.getFinishedRenderSemaphore( currentFrame );

    vk::SubmitInfo submitInfo( 1,                   // waitSemaphoreCount
                               &waitSemaphore,      // pWaitSemaphores
                               &pWaitDstStageMask,  // pWaitDstStageMask
                               1,                   // commandBufferCount
                               &cmdBuf,             // pCommandBuffers
                               1,                   // signalSemaphoreCount
                               &signaleSemaphore ); // pSignalSemaphores

    components::graphicsQueue.submit( submitInfo, currentInFlightFence_t );

    // Tell the presentation engine that the current image is ready.
    vk::PresentInfoKHR presentInfo( 1,                      // waitSemaphoreCount
                                    &signaleSemaphore,      // pWaitSemaphores
                                    1,                      // swapchainCount
                                    &components::swapchain, // pSwapchains
                                    &imageIndex,            // pImageIndices
                                    nullptr );              // pResults

    // This try catch block is only necesary on Linux for whatever reason. Without it, resizing the window will result in an unhandled throw of vk::Result::eErrorOutOfDateKHR.
    try
    {
      vk::Result result = components::graphicsQueue.presentKHR( presentInfo );
      if ( result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR )
      {
        _settings.triggerSwapchainRefresh( );
        RX_WARN( "Swapchain out of data or suboptimal." );
      }
    }
    catch ( ... )
    {
      _settings.triggerSwapchainRefresh( );
    }

    prevFrame    = currentFrame;
    currentFrame = ( currentFrame + 1 ) % _sync.getMaxFramesInFlight( );
  }

  void Api::updateSettings( )
  {
    if ( _settings._maxGeometryChanged || _settings._maxMeshesChanged || _settings._maxTexturesChanged )
    {
      _sync.waitForFrame( prevFrame );

      _settings._maxGeometryChanged = false;
      _settings._maxMeshesChanged   = false;
      _settings._maxTexturesChanged = false;

      _scene._vertexBuffers.resize( _settings._maxGeometry );
      _scene._indexBuffers.resize( _settings._maxGeometry );
      _scene._meshBuffers.resize( _settings._maxMeshes );
      _scene._textures.resize( _settings._maxTextures );

      // @todo check if this cant be just waiting for frame instead
      _scene.initGeoemtryDescriptorSets( );
      _scene.updateGeoemtryDescriptors( );

      _settings._refreshPipeline = true;
      //_settings._refreshSwapchain = true;
    }

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

  void Api::render( )
  {
    updateSettings( );

    update( );

    // If the window is minimized then simply do not render anything anymore.
    if ( _window->minimized( ) )
    {
      return;
    }

    // If the window size has changed the swapchain has to be recreated.
    if ( _window->changed( ) )
    {
      _camera->_updateProj = true;
      return;
    }

    prepareFrame( );

    recordSwapchainCommandBuffers( );

    submitFrame( );
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

    const auto& storageImageInfo = _pathTracer.getStorageImageInfo( );

    _postProcessingRenderer.updateDescriptors( storageImageInfo );

    _pathTracer.updateDescriptors( );

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

  void Api::initPipelines( )
  {
    RX_LOG_TIME_START( "Initializing graphic pipelines ..." );

    // path tracing pipeline
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = { _pathTracer.getDescriptorSetLayout( ),
                                                                  _scene._sceneDescriptors.layout.get( ),
                                                                  _scene._geometryDescriptors.layout.get( ) };

    _pathTracer.createPipeline( descriptorSetLayouts, &_settings );
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
    RX_ASSERT( _pathTracer.getPipeline( ) && _postProcessingRenderer.getPipeline( ), "Can not record swapchain command buffers because the pipelines have not been initialized yet." );

    // Wait for previous frame to finish command buffer execution.
    _sync.waitForFrame( prevFrame );

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

        size_t index = imageIndex % _sync.getMaxFramesInFlight( );

        std::vector<vk::DescriptorSet> descriptorSets = { _pathTracer.getDescriptorSet( index ),
                                                          _scene._sceneDescriptorsets[index],
                                                          _scene._geometryDescriptorSets[index] };

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
          _postProcessingRenderer.render( cmdBuf, _swapchain.getExtent( ), index );

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
} // namespace RAYEX_NAMESPACE
