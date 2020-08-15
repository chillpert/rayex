#include "Api.hpp"
#include "Components.hpp"
#include "Initializers.hpp"
#include "Helpers.hpp"
#include "Destructors.hpp"

namespace rx
{
  bool rayTrace = true;

  // Note: Vulkan default allocation limit = 4069
  const size_t maxGeometryNodes = 2000;
  const size_t maxTextures = 2000;
  const size_t maxLightNodes = 25;

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

    m_geometryNodes.reserve( maxGeometryNodes );
    m_textures.reserve( maxTextures );
    m_lightNodes.reserve( maxLightNodes );

    initInstance( );
    initDebugMessenger( );
    initSurface( );
    initPhysicalDevice( );
    initQueues( );
    initDevice( );
    initRenderPass( );
    initSwapchain( );
    initSwapchainImageViews( );
    initPipeline( );
    initGraphicsCommandPool( );
    initTransferCommandPool( );
    initDepthBuffering( );
    initSwapchainFramebuffers( );
    initDescriptorPool( );
    initRayTracing( );
    initSwapchainCommandBuffers( );
    initGui( );
    recordSwapchainCommandBuffers( );
    initSyncObjects( );

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

    // TODO: Temporary
    for ( std::shared_ptr<GeometryNode> node : m_geometryNodes )
    {
      if ( node->m_modelPath.empty( ) )
        continue;

      UniformBufferObject ubo { node->m_worldTransform, m_camera->getViewMatrix( ), m_camera->getProjectionMatrix( ), m_camera->getPosition( ) };
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
    m_geometryNodes.reserve( maxGeometryNodes );

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
    initSwapchainImageViews( );
    initDepthBuffering( );
    initSwapchainFramebuffers( );
    initSwapchainCommandBuffers( );
    recordSwapchainCommandBuffers( );

    if ( m_gui != nullptr )
      m_gui->recreate( &m_swapchain, vk::Helper::unpack( m_swapchainImageViews ) );

    // Update the camera screen size to avoid image stretching.
    auto screenSize = m_swapchain.getExtent( );
    m_camera->setScreenSize( glm::ivec2 { static_cast<int>( screenSize.width ), static_cast<int>( screenSize.height ) } );
    
    RX_LOG( "Finished swapchain recreation." );
  }

  void Api::initRayTracing( )
  {
    m_rayTracingBuilder.init( );
  }

  void Api::rayTrace( )
  {
    
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
      "VK_KHR_get_memory_requirements2",
      "VK_EXT_descriptor_indexing",
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
    // Reassess surface settings. 
    // TODO: This should be solved more elegantly. And change naming.
    m_surface.checkSettingSupport( );

    m_swapchain.init( &m_surface );
    m_swapchain.setImageAspect( vk::ImageAspectFlagBits::eColor );
  }

  void Api::initSwapchainImageViews( )
  {
    m_swapchainImageViews.resize( m_swapchain.getImages( ).size( ) );
    for ( size_t i = 0; i < m_swapchainImageViews.size( ); ++i )
    {
      m_swapchainImageViews[i] = vk::Initializer::createImageViewUnique( m_swapchain.getImage( i ), m_surface.getFormat( ), m_swapchain.getImageAspect( ) );
    }
  }

  void Api::initPipeline( bool firstRun )
  {
    // Create shaders.
    auto vs = vk::Initializer::createShaderModuleUnique( "shaders/simple3D.vert" );
    auto fs = vk::Initializer::createShaderModuleUnique( "shaders/simple3D.frag" );

    // TODO: Unnecessary check.
    if ( firstRun )
    {
      vk::DescriptorSetLayoutBinding vertexBinding( 0,                                                                      // binding
                                                    vk::DescriptorType::eUniformBuffer,                                     // descriptorType
                                                    1,                                                                      // descriptorCount
                                                    vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eRaygenKHR, // stageFlags
                                                    nullptr );                                                              // pImmutableSamplers

      vk::DescriptorSetLayoutBinding fragmentBinding( 1,                                                                            // binding
                                                      vk::DescriptorType::eCombinedImageSampler,                                    // descriptorType
                                                      1,                                                                            // descriptorCount
                                                      vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                                      nullptr );                                                                    // pImmutableSamplers

      m_descriptorSetLayout.addBinding( vertexBinding );
      m_descriptorSetLayout.addBinding( fragmentBinding );

      // Descriptor Set Layout
      m_descriptorSetLayout.init( );
    }

    glm::fvec2 extent = { static_cast<float>( m_swapchain.getExtent( ).width ), static_cast<float>( m_swapchain.getExtent( ).height ) };
    
    // Graphics pipeline
    m_pipeline.init( m_renderPass.get( ),
                     vk::Viewport { 0.0f, 0.0f, extent.x, extent.y, 0.0f, 1.0f },
                     { 0, { m_swapchain.getExtent( ).width, m_swapchain.getExtent( ).height } },
                     vs.get( ),
                     fs.get( ),
                     m_descriptorSetLayout.get( ) );

    // Ray tracing pipeline.
    auto rgen = vk::Initializer::createShaderModuleUnique( "shaders/raytrace.rgen" );
    auto miss = vk::Initializer::createShaderModuleUnique( "shaders/raytrace.rmiss" );
    auto chit = vk::Initializer::createShaderModuleUnique( "shaders/raytrace.rchit" );

    m_rayTracingBuilder.createDescriptorSetLayout( m_swapchain, maxGeometryNodes );

    m_rtPipeline.init( m_renderPass.get( ),
                       vk::Viewport { 0.0f, 0.0f, extent.x, extent.y, 0.0f, 1.0f },
                       { 0, { m_swapchain.getExtent( ).width, m_swapchain.getExtent( ).height } },
                       { m_rayTracingBuilder.getDescriptorSetLayout( ) },
                       rgen.get( ),
                       miss.get( ),
                       chit.get( ),
                       8 );
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

  void Api::initDepthBuffering( )
  {
    // Depth image for depth buffering
    vk::Format depthFormat = getSupportedDepthFormat( g_physicalDevice );

    auto imageCreateInfo = vk::Helper::getImageCreateInfo( vk::Extent3D( m_swapchain.getExtent( ).width, m_swapchain.getExtent( ).height, 1 ) );
    imageCreateInfo.format = depthFormat;
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

    m_depthImage.init( imageCreateInfo );

    // Image view for depth image
    m_depthImageView = vk::Initializer::createImageViewUnique( m_depthImage.get( ), depthFormat, vk::ImageAspectFlagBits::eDepth );
  }

  void Api::initSwapchainFramebuffers( )
  {
    m_swapchainFramebuffers.resize( m_swapchainImageViews.size( ) );
    for ( size_t i = 0; i < m_swapchainFramebuffers.size( ); ++i )
    {
      m_swapchainFramebuffers[i] = vk::Initializer::createFramebufferUnique( { m_swapchainImageViews[i].get( ), m_depthImageView.get( ) }, m_renderPass.get( ), m_swapchain.getExtent( ) );
    }
  }

  void Api::initDescriptorPool( )
  {
    uint32_t swapchainImagesCount = m_swapchain.getImages( ).size( );

    std::vector<vk::DescriptorPoolSize> poolSizes =
    {
      { vk::DescriptorType::eUniformBuffer, swapchainImagesCount },
      { vk::DescriptorType::eCombinedImageSampler, swapchainImagesCount }
    };

    m_descriptorPool = vk::Initializer::createDescriptorPoolUnique( poolSizes, maxGeometryNodes * swapchainImagesCount );
  }

  void Api::initModel( const std::shared_ptr<GeometryNode> node )
  {
    if ( node->m_modelPath.empty( ) )
    {
      RX_LOG( "Path to model is missing." );
      return;
    }

    auto it = m_models.find( node->m_modelPath );
    RX_ASSERT( ( it != m_models.end( ) ), "Model was not initialized previously." );

    auto model = it->second;
    if ( !model->m_initialized )
    {
      model->m_vertexBuffer.init( model->m_vertices );
      model->m_indexBuffer.init( model->m_indices );

      model->m_initialized = true;
      model->m_initialized = true;
    }

    node->m_uniformBuffers.init( m_swapchain.getImages( ).size( ) );

    // Create the descriptor set.
    model->m_descriptorSets.init( m_descriptorPool.get( ),
                                  static_cast<uint32_t>( m_swapchain.getImages( ).size( ) ),
                                  std::vector<vk::DescriptorSetLayout>( m_swapchain.getImages( ).size( ), m_descriptorSetLayout.get( ) ) );

    // TODO: add support for multiple textures.
    auto diffuseIter = m_textures.find( node->m_material.m_diffuseTexture );
    if ( diffuseIter != m_textures.end( ) )
    {
      model->m_descriptorSets.update( node->m_uniformBuffers.getRaw( ),
                                      diffuseIter->second->getImageView( ),
                                      diffuseIter->second->getSampler( ) );
    }

    // TODO: Try to call this as few times as possible
    m_rayTracingBuilder.createBottomLevelAS( vk::Helper::unpack( m_models ) );
    m_rayTracingBuilder.createTopLevelAS( m_geometryNodes );
    m_rayTracingBuilder.createDescriptorSet( m_swapchain );
    m_rayTracingBuilder.createShaderBindingTable( m_rtPipeline.get( ) );
  }

  void Api::initSwapchainCommandBuffers( )
  {
    m_swapchainCommandBuffers.init( m_graphicsCmdPool.get( ), m_swapchainFramebuffers.size( ), vk::CommandBufferUsageFlagBits::eRenderPassContinue );
  }

  void Api::initGui( )
  {
    if ( m_gui != nullptr )
    {
      initRenderPass( );
      m_gui->init( &m_surface, &m_swapchain, vk::Helper::unpack( m_swapchainImageViews ) );
    }
  }

  void Api::recordSwapchainCommandBuffers( )
  {
    RX_LOG( "Started swapchain command buffer recording." );

    // Set up render pass begin info
    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color = { std::array<float, 4>{ 0.5f, 0.5f, 0.5f, 1.0f } };
    clearValues[1].depthStencil = vk::ClearDepthStencilValue { 1.0f, 0 };

    // Start recording the swapchain framebuffers
    for ( size_t imageIndex = 0; imageIndex < m_swapchainCommandBuffers.get( ).size( ); ++imageIndex )
    {
      m_swapchainCommandBuffers.begin( imageIndex );

      m_renderPass.begin( m_swapchainFramebuffers[imageIndex].get( ),
                          m_swapchainCommandBuffers.get( imageIndex ),
                          { 0, m_swapchain.getExtent( ) },
                          { clearValues[0], clearValues[1] } );

      // TODO: Do push constants even make sense when you have got multiple light sources? Well, at least this is a working example.
      for ( const auto& lightNode : m_lightNodes )
      {
        if ( dynamic_cast<DirectionalLightNode*>( lightNode.get( ) ) )
        {
          auto ptr = std::dynamic_pointer_cast<DirectionalLightNode>( lightNode );

          auto pc = ptr->toPushConstant( );

          // TODO: avoid repetition of values.
          m_swapchainCommandBuffers.get( imageIndex ).pushConstants( m_pipeline.getLayout( ),            // layout
                                                                     vk::ShaderStageFlagBits::eFragment, // stageFlags
                                                                     0,                                  // offset
                                                                     sizeof( pc ),                       // size 
                                                                     &pc );                              // pValues
        }
      }

      m_pipeline.bind( m_swapchainCommandBuffers.get( imageIndex ) ); // CMD

      // Dynamic states
      vk::Viewport viewport = m_pipeline.getViewport( );
      viewport.width = m_window->getProperties( ).getWidth( );
      viewport.height = m_window->getProperties( ).getHeight( );

      m_swapchainCommandBuffers.get( imageIndex ).setViewport( 0, 1, &viewport ); // CMD

      vk::Rect2D scissor = m_pipeline.getScissor( );
      scissor.extent = m_window->getExtent( );

      m_swapchainCommandBuffers.get( imageIndex ).setScissor( 0, 1, &scissor ); // CMD

      // Draw models
      for ( const auto& node : m_geometryNodes )
      {
        if ( node->m_modelPath.empty( ) )
          continue;

        auto it = m_models.find( node->m_modelPath );
        RX_ASSERT( ( it != m_models.end( ) ), "Can not draw model because it was not found." );

        auto model = it->second;

        vk::Buffer vertexBuffers[] = { model->m_vertexBuffer.get( ) };
        vk::DeviceSize offsets[] = { 0 };

        m_swapchainCommandBuffers.get( imageIndex ).bindVertexBuffers( 0,               // first binding
                                                                       1,               // binding count
                                                                       vertexBuffers,
                                                                       offsets );

        m_swapchainCommandBuffers.get( imageIndex ).bindIndexBuffer( model->m_indexBuffer.get( ),
                                                                     0,                                 // offset
                                                                     model->m_indexBuffer.getType( ) );
        
        m_swapchainCommandBuffers.get( imageIndex ).bindDescriptorSets( vk::PipelineBindPoint::eGraphics,
                                                                        m_pipeline.getLayout( ),
                                                                        0,                                           // first set
                                                                        1,                                           // descriptor set count
                                                                        &model->m_descriptorSets.get( )[imageIndex], // descriptor sets
                                                                        0,                                           // dynamic offset count
                                                                        nullptr );                                   // dynamic offsets 

        m_swapchainCommandBuffers.get( imageIndex ).drawIndexed( model->m_indexBuffer.getCount( ), // index count
                                                                 1,                                // instance count
                                                                 0,                                // first index
                                                                 0,                                // vertex offset
                                                                 0 );                              // first instance
      }

      m_renderPass.end( m_swapchainCommandBuffers.get( imageIndex ) );
      m_swapchainCommandBuffers.end( imageIndex );
    }
  }

  void Api::initSyncObjects( )
  {
    // Synchronization
    m_imageAvailableSemaphores.resize( maxFramesInFlight );
    m_finishedRenderSemaphores.resize( maxFramesInFlight );
    m_inFlightFences.resize( maxFramesInFlight );
    m_imagesInFlight.resize( m_swapchain.getImages( ).size( ), nullptr );

    for ( size_t i = 0; i < maxFramesInFlight; ++i )
    {
      m_imageAvailableSemaphores[i] = vk::Initializer::createSemaphoreUnique( );
      m_finishedRenderSemaphores[i] = vk::Initializer::createSemaphoreUnique( );
      m_inFlightFences[i] = vk::Initializer::createFenceUnique( );
    }
  }
}
