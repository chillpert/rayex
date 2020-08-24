#include "Api.hpp"
#include "Components.hpp"
#include "Initializers.hpp"
#include "Helpers.hpp"
#include "Destructors.hpp"

namespace rx
{
  bool rayTraceOn = true;

  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  Api::Api( std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera ) :
    m_window( window ),
    m_camera( camera ),
    m_gui( nullptr )
  { }

  Api::Api( std::shared_ptr<WindowBase> window, std::shared_ptr<GuiBase> gui, std::shared_ptr<CameraBase> camera ) :
    m_window( window ),
    m_camera( camera ),
    m_gui( gui )
  { }

  Api::~Api( )
  {
    clean( );

    // Gui needs to be destroyed manually, as RAII destruction will not be possible.
    if ( m_gui != nullptr )
      m_gui->destroy( );
  }

  void Api::init( )
  {
    g_window = m_window;

    m_geometryNodes.reserve( g_maxGeometryNodes );
    m_textures.reserve( g_maxTextures );
    m_lightNodes.reserve( g_maxLightNodes );

    initInstance( );
    initDebugMessenger( );
    initSurface( );
    initPhysicalDevice( );
    initQueues( );
    initDevice( );
    initRenderPass( );
    initSwapchain( );
    initPipeline( );
    initGraphicsCommandPool( );
    initTransferCommandPool( );
    initRayTracing( );
    initSwapchainCommandBuffers( );
    initGui( );
    recordSwapchainCommandBuffers( );
    initSyncObjects( );

    // Temporary: Make sure swapchain images are presentable.
    m_swapchain.setImageLayout( vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR );

    RX_LOG( "Finished API initialization." );
  }

  bool Api::update( )
  {
    return true;
  }

  bool Api::render( )
  {
    if ( m_gui != nullptr )
    {
      m_gui->beginRender( );
      m_gui->render( );
      m_gui->endRender( );
    }

    static size_t currentFrame = 0;

    // Wait for the current frame's fences.
    g_device.waitForFences( 1, &m_inFlightFences[currentFrame].get( ), VK_TRUE, UINT64_MAX );

    // If the window is minimized then simply do not render anything anymore.
    if ( m_window->minimized( ) )
      return true;

    // If the window size has changed the swapchain has to be recreated.
    if ( m_window->changed( ) || m_recreateSwapchain )
    {
      m_recreateSwapchain = false;
      recreateSwapchain( );
      return true;
    }

    uint32_t imageIndex;
    m_swapchain.acquireNextImage( m_imageAvailableSemaphores[currentFrame].get( ), nullptr, &imageIndex );

    // TODO: This should only be called if the camera was changed. 
    for ( std::shared_ptr<GeometryNode> node : m_geometryNodes )
    {
      if ( node->m_modelPath.empty( ) )
        continue;

      UniformBufferObject ubo { node->m_worldTransform, m_camera->getViewMatrix( ), m_camera->getProjectionMatrix( ), m_camera->getViewInverseMatrix( ), m_camera->getProjectionInverseMatrix( ), m_camera->getPosition( ) };
      node->m_uniformBuffers.upload( imageIndex, ubo );
    }

    // Check if a previous frame is using the current image.
    if ( static_cast<void*>( m_imagesInFlight[imageIndex] ) != nullptr )
      g_device.waitForFences( 1, &m_imagesInFlight[currentFrame], VK_TRUE, UINT64_MAX );

    // This will mark the current image to be in use by this frame.
    m_imagesInFlight[imageIndex] = m_inFlightFences[currentFrame].get( );

    std::vector<vk::CommandBuffer> commandBuffers = { m_swapchainCommandBuffers.get( )[imageIndex] };
    if ( m_gui != nullptr )
      commandBuffers.push_back( m_gui->getCommandBuffer( imageIndex ) );

    vk::SubmitInfo submitInfo( 1,                                                                                                  // waitSemaphoreCount
                               &m_imageAvailableSemaphores[currentFrame].get( ),                                                   // pWaitSemaphores
                               std::array<vk::PipelineStageFlags, 1>{ vk::PipelineStageFlagBits::eColorAttachmentOutput }.data( ), // pWaitDstStageMask
                               static_cast<uint32_t>( commandBuffers.size( ) ),                                                    // commandBufferCount
                               commandBuffers.data( ),                                                                             // pCommandBuffers
                               1,                                                                                                  // signalSemaphoreCount
                               &m_finishedRenderSemaphores[currentFrame].get( ) );                                                 // pSignalSemaphores

    // Reset the signaled state of the current frame's fence to the unsignaled one.
    g_device.resetFences( 1, &m_inFlightFences[currentFrame].get( ) );

    if ( m_gui != nullptr )
    {
      m_gui->beginRenderPass( imageIndex );
      m_gui->endRenderPass( imageIndex );
    }

    // Submits / executes the current image's / framebuffer's command buffer.
    g_graphicsQueue.submit( submitInfo, m_inFlightFences[currentFrame].get( ) );

    vk::PresentInfoKHR presentInfo( 1,                                                // waitSemaphoreCount
                                    &m_finishedRenderSemaphores[currentFrame].get( ), // pWaitSemaphores
                                    1,                                                // swapchainCount
                                    &g_swapchain,                                     // pSwapchains
                                    &imageIndex,                                      // pImageIndices
                                    nullptr );                                        // pResults

    // Tell the presentation engine that the current image is ready.
    g_graphicsQueue.presentKHR( presentInfo );

    currentFrame = ( currentFrame + 1 ) % maxFramesInFlight;

    return true;
  }

  void Api::pushNode( const std::shared_ptr<Node> node, bool record )
  {
    if ( dynamic_cast<GeometryNode*>( node.get( ) ) )
    {
      auto ptr = std::dynamic_pointer_cast<GeometryNode>( node );

      auto it = m_models.find( ptr->m_modelPath );
      if ( it == m_models.end( ) )
        m_models.insert( { ptr->m_modelPath, std::make_shared<Model>( ptr->m_modelPath ) } );

      m_geometryNodes.push_back( ptr );

      // Handle the node's texture.
      auto texturePaths = ptr->m_material.getTextures( );

      for ( const auto& texturePath : texturePaths )
      {
        auto it = m_textures.find( texturePath );
        // Texture does not exist already. It will be created.
        if ( it == m_textures.end( ) )
        {
          m_textures.insert( { texturePath, std::make_shared<Texture>( texturePath ) } );
        }
      }

      if ( record )
        initModel( ptr );
    }
    else if ( dynamic_cast<LightNode*>( node.get( ) ) )
    {
      auto lightNodePtr = std::dynamic_pointer_cast<LightNode>( node );

      m_lightNodes.push_back( lightNodePtr );
    }

    if ( record )
    {
      m_swapchainCommandBuffers.reset( );
      recordSwapchainCommandBuffers( );
    }
  }

  void Api::setNodes( const std::vector<std::shared_ptr<Node>>& nodes )
  {
    m_geometryNodes.clear( );
    m_geometryNodes.reserve( g_maxGeometryNodes );

    for ( const auto& node : nodes )
      pushNode( node );

    m_swapchainCommandBuffers.reset( );
    recordSwapchainCommandBuffers( );
  }

  void Api::clean( )
  {
    g_device.waitIdle( );
  }

  void Api::recreateSwapchain( )
  {
    RX_LOG( "Recreating swapchain." );

    g_device.waitIdle( );

    // Clean up existing swapchain and dependencies.
    m_swapchainCommandBuffers.free( );
    m_swapchain.destroy( );

    // Recreating the swapchain.
    initSwapchain( );
    initSwapchainCommandBuffers( );
    recordSwapchainCommandBuffers( );

    if ( m_gui != nullptr )
      m_gui->recreate( m_swapchain.getExtent( ), m_swapchain.getImageViews( ) );

    // Update the camera screen size to avoid image stretching.
    auto screenSize = m_swapchain.getExtent( );
    m_camera->setScreenSize( glm::ivec2 { static_cast<int>( screenSize.width ), static_cast<int>( screenSize.height ) } );
    
    RX_LOG( "Finished swapchain recreation." );
  }

  void Api::initRayTracing( )
  {
    m_rayTracingBuilder.init( );

    m_rayTracingBuilder.createStorageImage( m_swapchain.getExtent( ) );
  }

  void Api::initInstance( )
  {
    std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };
  #ifdef RX_DEBUG
    std::vector<const char*> extensions = { "VK_KHR_get_physical_device_properties2", "VK_EXT_debug_utils" };
  #elif
    std::vector<const char*> extensions = { "VK_KHR_get_physical_device_properties2" };
  #endif
    m_instance.init( layers, extensions );
  }

  void Api::initDebugMessenger( )
  {
    m_debugMessenger.init( vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );
  }

  void Api::initSurface( )
  {
    m_surface.init( );
  }

  void Api::initPhysicalDevice( )
  {
    m_physicalDevice.init( );

    // Reassess the support of the preferred surface settings.
    m_surface.checkSettingSupport( );
  }

  void Api::initQueues( )
  {
    m_queues.init( );
  }

  void Api::initDevice( )
  {
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

    m_device.init( deviceExtensions );

    // Retrieve all queue handles.
    m_queues.retrieveHandles( );
  }

  void Api::initRenderPass( )
  {
    vk::AttachmentDescription colorAttachmentDescription( { },                                                                                              // flags
                                                          m_surface.getFormat( ),                                                                           // format
                                                          vk::SampleCountFlagBits::e1,                                                                      // samples
                                                          vk::AttachmentLoadOp::eClear,                                                                     // loadOp
                                                          vk::AttachmentStoreOp::eStore,                                                                    // storeOp
                                                          vk::AttachmentLoadOp::eDontCare,                                                                  // stencilLoadOp
                                                          vk::AttachmentStoreOp::eDontCare,                                                                 // stencilStoreOp
                                                          vk::ImageLayout::eUndefined,                                                                      // initialLayout
                                                          m_gui != nullptr ? vk::ImageLayout::eColorAttachmentOptimal : vk::ImageLayout::ePresentSrcKHR );  // finalLayout    

    vk::AttachmentReference colorAttachmentReference( 0,                                          // attachment
                                                      vk::ImageLayout::eColorAttachmentOptimal ); // layout

    vk::AttachmentDescription depthAttachmentDescription( { },                                                // flags
                                                          getSupportedDepthFormat( g_physicalDevice ),        // format
                                                          vk::SampleCountFlagBits::e1,                        // samples
                                                          vk::AttachmentLoadOp::eClear,                       // loadOp
                                                          vk::AttachmentStoreOp::eStore,                      // storeOp
                                                          vk::AttachmentLoadOp::eClear,                       // stencilLoadOp
                                                          vk::AttachmentStoreOp::eDontCare,                   // stencilStoreOp
                                                          vk::ImageLayout::eUndefined,                        // initialLayout
                                                          vk::ImageLayout::eDepthStencilAttachmentOptimal );  // finalLayout    

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

    m_renderPass.init( { colorAttachmentDescription, depthAttachmentDescription }, { subpassDescription }, subpassDependencies );
  }

  void Api::initSwapchain( )
  {
    m_swapchain.init( &m_surface, m_renderPass.get( ) );
  }

  void Api::initPipeline( bool firstRun )
  {
    glm::fvec2 extent = { static_cast<float>( m_swapchain.getExtent( ).width ), static_cast<float>( m_swapchain.getExtent( ).height ) };

    // Initialize the ray tracing pipeline.
    auto rgen = vk::Initializer::createShaderModuleUnique( "shaders/raytrace.rgen" );
    auto miss = vk::Initializer::createShaderModuleUnique( "shaders/raytrace.rmiss" );
    auto chit = vk::Initializer::createShaderModuleUnique( "shaders/raytrace.rchit" );

    // Also creates the bindings.
    m_rayTracingBuilder.createDescriptorSetLayout( );
    //m_rayTracingBuilder.createSceneDescriptorSetLayout( );

    m_rtPipeline.init( vk::Viewport { 0.0f, 0.0f, extent.x, extent.y, 0.0f, 1.0f },
                       { 0, { m_swapchain.getExtent( ).width, m_swapchain.getExtent( ).height } },
                       { m_rayTracingBuilder.getDescriptorSetLayout( ) },// m_rayTracingBuilder.getSceneDescriptorSetLayout( ) },
                       rgen.get( ),
                       miss.get( ),
                       chit.get( ) ); // TODO: this should be exposed to client in some config data structure.
  }

  void Api::initGraphicsCommandPool( )
  {
    m_graphicsCmdPool = vk::Initializer::createCommandPoolUnique( g_graphicsFamilyIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
    g_graphicsCmdPool = m_graphicsCmdPool.get( );
  }

  void Api::initTransferCommandPool( )
  {
    m_transferCmdPool = vk::Initializer::createCommandPoolUnique( g_transferFamilyIndex, { } );
    g_transferCmdPool = m_transferCmdPool.get( );
  }

  void Api::initModel( const std::shared_ptr<GeometryNode> node )
  {
    auto it = m_models.find( node->m_modelPath );
    auto model = it->second;
    if ( !model->m_initialized )
    {
      model->m_vertexBuffer.init( model->m_vertices );
      model->m_indexBuffer.init( model->m_indices );

      model->m_initialized = true;
    }

    // TODO: Try to call this as few times as possible
    m_rayTracingBuilder.createBottomLevelAS( vk::Helper::unpack( m_models ) );
    m_rayTracingBuilder.createTopLevelAS( m_geometryNodes );
    m_rayTracingBuilder.createShaderBindingTable( m_rtPipeline.get( ) );

    node->m_uniformBuffers.init( g_swapchainImageCount );

    // Create the ray tracing descriptor sets.
    uint32_t swapchainImagesCount = static_cast<uint32_t>( g_swapchainImageCount );
    m_rayTracingBuilder.createDescriptorPool( swapchainImagesCount );
    m_rayTracingBuilder.createDescriptorSets( swapchainImagesCount );

    m_rayTracingBuilder.updateDescriptorSets( node->m_uniformBuffers.getRaw( ), model->m_vertexBuffer.get( ), model->m_indexBuffer.get( ) );
  }

  void Api::initSwapchainCommandBuffers( )
  {
    m_swapchainCommandBuffers.init( m_graphicsCmdPool.get( ), 
                                    g_swapchainImageCount, 
                                    vk::CommandBufferUsageFlagBits::eRenderPassContinue );
  }

  void Api::initGui( )
  {
    if ( m_gui != nullptr )
    {
      initRenderPass( );
      m_gui->init( &m_surface, m_swapchain.getExtent( ), m_swapchain.getImageViews( ) );
    }
  }

  void Api::recordSwapchainCommandBuffers( )
  {
    RX_LOG( "Started swapchain command buffer recording." );

    // Start recording the swapchain framebuffers
    for ( size_t imageIndex = 0; imageIndex < m_swapchainCommandBuffers.get( ).size( ); ++imageIndex )
    {
      m_swapchainCommandBuffers.begin( imageIndex );

      m_rtPipeline.bind( m_swapchainCommandBuffers.get( imageIndex ) );
        
      for ( const auto& node : m_geometryNodes )
      {
        std::vector<vk::DescriptorSet> descriptorSets = { m_rayTracingBuilder.getDescriptorSets( ).get( imageIndex ) }; // , m_rayTracingBuilder.getSceneDescriptorSets( ).get( imageIndex ) };

        m_swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eRayTracingKHR,
                                                                        m_rtPipeline.getLayout( ),
                                                                        0,                                                           // first set
                                                                        static_cast<uint32_t>( descriptorSets.size( ) ),             // descriptor set count
                                                                        descriptorSets.data( ),                                      // descriptor sets
                                                                        0,                                                           // dynamic offset count
                                                                        nullptr );                                                   // dynamic offsets 

        m_rayTracingBuilder.m_rtPushConstants.clearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
        m_rayTracingBuilder.m_rtPushConstants.lightPosition = { 10.0f, 10.0f, 0.0f };
        m_rayTracingBuilder.m_rtPushConstants.lightIntensity = 2.5f;
        m_rayTracingBuilder.m_rtPushConstants.lightType = 1;

        m_swapchainCommandBuffers.get( imageIndex ).pushConstants<RayTracingBuilder::PushConstant>( m_rtPipeline.getLayout( ), 
                                                                                                    vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR | vk::ShaderStageFlagBits::eMissKHR, 
                                                                                                    0,
                                                                                                    m_rayTracingBuilder.m_rtPushConstants );

        m_rayTracingBuilder.rayTrace( m_swapchainCommandBuffers.get( imageIndex ), m_swapchain.getImage( imageIndex ), m_window->getExtent( ) );
      }

      m_swapchainCommandBuffers.end( imageIndex );
    }
  }

  void Api::initSyncObjects( )
  {
    // Synchronization
    m_imageAvailableSemaphores.resize( maxFramesInFlight );
    m_finishedRenderSemaphores.resize( maxFramesInFlight );
    m_inFlightFences.resize( maxFramesInFlight );
    m_imagesInFlight.resize( g_swapchainImageCount, nullptr );

    for ( size_t i = 0; i < maxFramesInFlight; ++i )
    {
      m_imageAvailableSemaphores[i] = vk::Initializer::createSemaphoreUnique( );
      m_finishedRenderSemaphores[i] = vk::Initializer::createSemaphoreUnique( );
      m_inFlightFences[i] = vk::Initializer::createFenceUnique( );
    }
  }
}
