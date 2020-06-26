#include "Api.hpp"

namespace RX
{
  const size_t maxNodes = 4096;

  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  Api::Api(std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera) :
    m_window(window),
    m_camera(camera),
    m_gui(nullptr) { }

  Api::Api(std::shared_ptr<WindowBase> window, std::shared_ptr<GuiBase> gui, std::shared_ptr<CameraBase> camera) :
    m_window(window),
    m_camera(camera),
    m_gui(gui) { }

  Api::~Api()
  {
    clean();

    // Gui needs to be destroyed manually, as RAII destruction will not be possible.
    if (m_gui != nullptr)
      m_gui->destroy();
  }

  void Api::init()
  {
    m_nodes.reserve(maxNodes);
    m_textures.reserve(maxNodes);

    initInstance();
    m_dispatchLoaderDynamic = vk::DispatchLoaderDynamic(m_instance.get(), vkGetInstanceProcAddr);
    initDebugMessenger();
    initSurface();
    initPhysicalDevice();
    initQueues();
    initDevice();
    initRenderPass();
    initSwapchain();
    initSwapchainImageViews();
    initPipeline();
    initgraphicsCmdPool();
    inittransferCmdPool();
    initDepthBuffering();
    initSwapchainFramebuffers();
    initDescriptorPool();
    initRayTracing();
    //initModels();
    initSwapchainCmdBuffers();
    initGui();
    recordSwapchainCommandBuffers();

    // Synchronization
    m_imageAvailableSemaphores.resize(maxFramesInFlight);
    m_finishedRenderSemaphores.resize(maxFramesInFlight);
    m_inFlightFences.resize(maxFramesInFlight);
    m_imagesInFlight.resize(m_swapchain.getImages().size(), VK_NULL_HANDLE);

    SemaphoreInfo semaphoreInfo{ };
    semaphoreInfo.device = m_device.get();

    FenceInfo fenceInfo{ };
    fenceInfo.device = m_device.get();

    for (size_t i = 0; i < maxFramesInFlight; ++i)
    {
      m_imageAvailableSemaphores[i].init(semaphoreInfo);
      m_finishedRenderSemaphores[i].init(semaphoreInfo);
      m_inFlightFences[i].init(fenceInfo);
    }

    RX_LOG("Finished API initialization.");
  }

  bool Api::update()
  {
    return true;
  }

  bool Api::render()
  {
    if (m_gui != nullptr)
    {
      m_gui->beginRender();
      m_gui->render();
      m_gui->endRender();
    }

    static size_t currentFrame = 0;

    // Wait for the current frame's fences.
    m_device.get().waitForFences(1, &m_inFlightFences[currentFrame].get(), VK_TRUE, UINT64_MAX);

    // If the window is minimized then simply do not render anything anymore.
    if (m_window->minimized())
      return true;

    // If the window size has changed the swapchain has to be recreated.
    if (m_window->changed() || m_recreateSwapchain)
    {
      m_recreateSwapchain = false;
      recreateSwapchain();
      return true;
    }

    uint32_t imageIndex;
    m_swapchain.acquireNextImage(m_imageAvailableSemaphores[currentFrame].get(), nullptr, &imageIndex);

    // TODO: Temporary
    for (std::shared_ptr<GeometryNodeBase> node : m_nodes)
    {
      if (node->m_modelPath.empty())
        continue;

      UniformBufferObject ubo
      {
        node->m_worldTransform,
        m_camera->getViewMatrix(),
        m_camera->getProjectionMatrix(),
        m_camera->getPosition()
      };
      
      node->m_uniformBuffers.upload(imageIndex, ubo);
    }

    // Check if a previous frame is using the current image.
    if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
      vkWaitForFences(m_device.get(), 1, &m_imagesInFlight[currentFrame], VK_TRUE, UINT64_MAX);

    // This will mark the current image to be in use by this frame.
    m_imagesInFlight[imageIndex] = m_inFlightFences[currentFrame].get();

    vk::SubmitInfo submitInfo;

    vk::Semaphore waitSemaphores[] = { m_imageAvailableSemaphores[currentFrame].get() };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    std::vector<vk::CommandBuffer> commandBuffers = { m_swapchainCmdBuffers.get()[imageIndex] };
    if (m_gui != nullptr)
      commandBuffers.push_back(m_gui->getCommandBuffer().get()[imageIndex]);

    submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    submitInfo.pCommandBuffers = commandBuffers.data();

    vk::Semaphore signalSemaphores[] = { m_finishedRenderSemaphores[currentFrame].get() };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Reset the signaled state of the current frame's fence to the unsignaled one.
    m_device.get().resetFences(1, &m_inFlightFences[currentFrame].get());

    if (m_gui != nullptr)
    {
      m_gui->beginRenderPass(imageIndex);
      m_gui->endRenderPass(imageIndex);
    }

    // Submits / executes the current image's / framebuffer's command buffer.
    m_queueManager.submit(submitInfo, m_inFlightFences[currentFrame].get());

    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = { m_swapchain.get() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    // Tell the presentation engine that the current image is ready.
    m_queueManager.present(presentInfo);

    currentFrame = (currentFrame + 1) % maxFramesInFlight;

    return true;
  }
  
  void Api::pushNode(const std::shared_ptr<GeometryNodeBase> node, bool record)
  {
    bool isNew = false;

    auto it = m_models.find(node->m_modelPath);
    if (it == m_models.end())
    {
      m_models.insert({ node->m_modelPath, std::make_shared<Model>(node->m_modelPath) });
      isNew = true;
    }

    m_nodes.push_back(node);

    // Handle the node's texture.
    auto texturePaths = node->m_material.getTextures();

    TextureInfo textureInfo{
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .commandPool = m_graphicsCmdPool.get(),
      .queue = m_queueManager.getQueue(GRAPHICS)->get()
    };

    for (const auto& texturePath : texturePaths)
    {
      auto it = m_textures.find(texturePath);
      // Texture does not exist already. It will be created.
      if (it == m_textures.end())
      {
        textureInfo.path = texturePath;
        m_textures.insert({ texturePath, std::make_shared<Texture>(textureInfo) });
      }
    }

    if (record)
    {
      initModel(node);
      m_swapchainCmdBuffers.reset();
      recordSwapchainCommandBuffers();
    }

    if (record && isNew)
    {

    }
  }

  void Api::setNodes(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes)
  {
    //m_nodes.erase(m_nodes.begin(), m_nodes.end());
    m_nodes.clear();
    m_nodes.reserve(maxNodes);

    for (const auto& node : nodes)
      pushNode(node, false);

    initModels(true);
    m_swapchainCmdBuffers.reset();
    recordSwapchainCommandBuffers();
  }

  void Api::clean()
  {
    m_device.waitIdle();
  }

  void Api::recreateSwapchain()
  {
    RX_LOG("Recreating swapchain.");

    m_device.waitIdle();

    // Cleaning the existing swapchain.
    m_depthImageView.destroy();
    m_depthImage.destroy();

    for (Framebuffer& framebuffer : m_swapchainFramebuffers)
      framebuffer.destroy();

    m_swapchainCmdBuffers.free();
    //m_pipeline.destroy();
    m_renderPass.destroy();

    for (ImageView& imageView : m_swapchainImageViews)
      imageView.destroy();

    m_swapchain.destroy();

    for (std::shared_ptr<GeometryNodeBase> node : m_nodes)
    {
      if (node->m_modelPath.empty())
        continue;

      //node->m_uniformBuffers.destroy();
    }

    //m_descriptorPool.destroy();

    // Recreating the swapchain.
    // Swapchain
    // TODO: the new swapchain's extent should be set using the framebuffer width and height stored in the window.
    initSwapchain();

    // Image views for swapchain images
    initSwapchainImageViews();
    initRenderPass();
    //initPipeline(false);
    initDepthBuffering();
    initSwapchainFramebuffers();
    //initDescriptorPool();
    initModels(false);
    initSwapchainCmdBuffers();
    recordSwapchainCommandBuffers();

    if (m_gui != nullptr)
    {
      GuiRecreateInfo guiRecreateInfo{ };
      guiRecreateInfo.minImageCount = m_surface.getCapabilities().minImageCount + 1;
      guiRecreateInfo.imageCount = static_cast<uint32_t>(m_swapchain.getImages().size());
      guiRecreateInfo.swapchainImageFormat = m_surface.getFormat();
      guiRecreateInfo.swapchainImageExtent = m_swapchain.getExtent();

      std::vector<vk::ImageView> temp(m_swapchainImageViews.size());

      for (size_t i = 0; i < m_swapchainImageViews.size(); ++i)
        temp[i] = m_swapchainImageViews[i].get();

      guiRecreateInfo.swapchainImageViews = temp;
      m_gui->recreate(guiRecreateInfo);
    }

    // Update the camera screen size to avoid image stretching.
    auto screenSize = m_swapchain.getExtent();
    int screenWidth = static_cast<int>(screenSize.width);
    int screenHeight = static_cast<int>(screenSize.height);

    m_camera->setScreenSize(glm::ivec2{ screenWidth, screenHeight });

    RX_LOG("Finished swapchain recreation.");
  }

  void Api::initRayTracing()
  {
    RaytraceBuilderInfo info{
      .instance = m_instance.get(),
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .surface = &m_surface,
      .queue = m_queueManager.getQueue(GRAPHICS),
      .dispatchLoaderDynamic = m_dispatchLoaderDynamic
    };

    m_raytraceBuilder.init(info);
  }

  void Api::initInstance()
  {
    m_instance.init({
        .window = m_window,
#ifdef RX_DEBUG
        .layers = { "VK_LAYER_KHRONOS_validation" },
        .extensions = { "VK_KHR_get_physical_device_properties2", "VK_EXT_debug_utils" }
#elif
        .extensions = { "VK_KHR_get_physical_device_properties2" }
#endif
      }
    );
  }

  void Api::initDebugMessenger()
  {
    m_debugMessenger.init({ m_instance.get() });
  }

  void Api::initSurface()
  {
    m_surface.init({ m_window.get(), m_instance.get() });
  }

  void Api::initPhysicalDevice()
  {
    m_physicalDevice.init({ m_instance.get(), m_surface.get() });

    // Reassess the support of the preferred surface settings.
    m_surface.checkSettingSupport(m_physicalDevice.get());
  }

  void Api::initQueues()
  {
    QueuesInfo queuesInfo{
      .physicalDevice = m_physicalDevice.get(),
      .surface = m_surface.get()
    };

    m_queueManager.init(queuesInfo);
#ifdef RX_DEBUG
    m_queueManager.print();
#endif
  }

  void Api::initDevice()
  {
    m_device.init({
        .physicalDevice = m_physicalDevice.get(),
        .queueFamilies = m_queueManager.getQueueFamilies(),
        .extensions = {
          "VK_KHR_get_memory_requirements2",
          "VK_EXT_descriptor_indexing",
          VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
          VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
          VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
          VK_KHR_MAINTENANCE3_EXTENSION_NAME,
          VK_KHR_RAY_TRACING_EXTENSION_NAME
        }
      }
    );

    // Retrieve all queue handles.
    m_queueManager.retrieveAllHandles(m_device.get());
  }

  void Api::initRenderPass()
  {
    vk::AttachmentDescription colorAttachmentDescription{
      { },                                                                                            // flags
      m_surface.getFormat(),                                                                          // format
      vk::SampleCountFlagBits::e1,                                                                    // samples
      vk::AttachmentLoadOp::eClear,                                                                   // loadOp
      vk::AttachmentStoreOp::eStore,                                                                  // storeOp
      vk::AttachmentLoadOp::eDontCare,                                                                // stencilLoadOp
      vk::AttachmentStoreOp::eDontCare,                                                               // stencilStoreOp
      vk::ImageLayout::eUndefined,                                                                    // initialLayout
      m_gui != nullptr ? vk::ImageLayout::eColorAttachmentOptimal : vk::ImageLayout::ePresentSrcKHR   // finalLayout
    };

    vk::AttachmentReference colorAttachmentReference{
      0,                                        // attachment
      vk::ImageLayout::eColorAttachmentOptimal  // layout
    };

    vk::AttachmentDescription depthAttachmentDescription{
      { },                                             // flags
      getSupportedDepthFormat(m_physicalDevice.get()), // format
      vk::SampleCountFlagBits::e1,                     // samples
      vk::AttachmentLoadOp::eClear,                    // loadOp
      vk::AttachmentStoreOp::eDontCare,                // storeOp
      vk::AttachmentLoadOp::eDontCare,                 // stencilLoadOp
      vk::AttachmentStoreOp::eDontCare,                // stencilStoreOp
      vk::ImageLayout::eUndefined,                     // initialLayout
      vk::ImageLayout::eDepthStencilAttachmentOptimal, // finalLayout
    };

    vk::AttachmentReference depthAttachmentRef{
      1,                                              // attachment
      vk::ImageLayout::eDepthStencilAttachmentOptimal // layout
    };

    vk::SubpassDescription subpassDescription{
      { },                              // flags
      vk::PipelineBindPoint::eGraphics, // pipelineBindPoint
      0,                                // inputAttachmentCount
      nullptr,                          // pInputAttachments
      1,                                // colorAttachmentCount
      &colorAttachmentReference,        // pColorAttachments
      nullptr,                          // pResolveAttachments
      &depthAttachmentRef,              // pDepthStencilAttachment
      0,                                // preserveAttachmentCount
      nullptr                           // pPreserveAttachments
    };

    vk::SubpassDependency subpassDependency{
      VK_SUBPASS_EXTERNAL,                                // srcSubpass
      0,                                                  // dstSubpass
      vk::PipelineStageFlagBits::eColorAttachmentOutput,  // srcStageMask
      vk::PipelineStageFlagBits::eColorAttachmentOutput,  // dstStageMask
      {},                                                 // srcAccessMask
      vk::AccessFlagBits::eColorAttachmentWrite,          // dstAccessMask 
      {}                                                  // dependencyFlags
    };

    m_renderPass.init({
        .physicalDevice = m_physicalDevice.get(),
        .device = m_device.get(),
        .attachments = { colorAttachmentDescription, depthAttachmentDescription },
        .subpasses = { subpassDescription },
        .dependencies = { subpassDependency }
      }
    );
  }

  void Api::initSwapchain()
  {
    m_surface.checkSettingSupport(m_physicalDevice.get());

    SwapchainInfo swapchainInfo{
      .window = m_window.get(),
      .surface = &m_surface,
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .queueFamilyIndices = m_queueManager.getQueueFamilyIndicesForSwapchainAccess(),
      .imageAspect = vk::ImageAspectFlagBits::eColor,
      .renderPass = m_renderPass.get()
    };

    m_swapchain.init(swapchainInfo);
  }

  void Api::initSwapchainImageViews()
  {
    m_swapchainImageViews.resize(m_swapchain.getImages().size());
    for (size_t i = 0; i < m_swapchainImageViews.size(); ++i)
    {
      ImageViewInfo imageViewInfo{
        .device = m_device.get(),
        .image = m_swapchain.getImage(i),
        .format = m_surface.getFormat(),
      };

      imageViewInfo.subresourceRange.aspectMask = m_swapchain.getImageAspect();

      m_swapchainImageViews[i].init(imageViewInfo);
    }
  }

  void Api::initPipeline(bool firstRun)
  {
    // Create shaders.
    Shader vs({
        .fullPath = RX_SHADER_PATH "simple3D.vert",
        .device = m_device.get()
      }
    );

    Shader fs({
        .fullPath = RX_SHADER_PATH "simple3D.frag",
        .device = m_device.get()
      }
    );

    if (firstRun)
    {
      vk::DescriptorSetLayoutBinding vertexBinding{
        0,                                  // binding
        vk::DescriptorType::eUniformBuffer, // descriptorType
        1,                                  // descriptorCount
        vk::ShaderStageFlagBits::eVertex,   // stageFlags
        nullptr
      };

      vk::DescriptorSetLayoutBinding fragmentBinding{
        1,                                          // binding
        vk::DescriptorType::eCombinedImageSampler,  // descriptorType
        1,                                          // descriptorCount
        vk::ShaderStageFlagBits::eFragment,         // stageFlags
        nullptr
      };

      m_descriptorSetLayout.addBinding(vertexBinding);
      m_descriptorSetLayout.addBinding(fragmentBinding);

      // Descriptor Set Layout
      m_descriptorSetLayout.init({
          .device = m_device.get()
        }
      );
    }

    // Graphics pipeline
    RasterizationPipelineInfo pipelineInfo{
      .vertexShader = vs.get(),
      .fragmentShader = fs.get(),
      .descriptorSetLayout = m_descriptorSetLayout.get(),
      .topology = vk::PrimitiveTopology::eTriangleList,
    };

    pipelineInfo.device = m_device.get();
    pipelineInfo.renderPass = m_renderPass.get();
    pipelineInfo.viewport = vk::Viewport{ 0.0f, 0.0f, static_cast<float>(m_swapchain.getExtent().width), static_cast<float>(m_swapchain.getExtent().height), 0.0f, 1.0f };
    pipelineInfo.scissor = { 0, 0, m_swapchain.getExtent().width, m_swapchain.getExtent().height };

    m_pipeline.init(pipelineInfo);
  }

  void Api::initgraphicsCmdPool()
  {
    CommandPoolInfo commandPoolInfo{
      .device = m_device.get(),
      .queueFamilyIndex = m_queueManager.getGraphicsFamilyIndex(),
      .createFlags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer
    };

    m_graphicsCmdPool.init(commandPoolInfo);
  }

  void Api::inittransferCmdPool()
  {
    // With the given parameters the function will return two different queue family indices. 
    // This means that the transfer queue family index will be a different one than the graphics queue family index.
    // However, this will only be the case if the queue family indices on this device allow it.
    static auto indices = m_queueManager.getUniqueQueueIndices({ GRAPHICS, TRANSFER });
    
    CommandPoolInfo commandPoolInfo{
      .device = m_device.get(),
      .queueFamilyIndex = indices.size() > 1 ? indices[1] : m_queueManager.getTransferFamilyIndex()
    };

    m_transferCmdPool.init(commandPoolInfo);
  }

  void Api::initDepthBuffering()
  {
    // Depth image for depth buffering
    vk::Format depthFormat = getSupportedDepthFormat(m_physicalDevice.get());

    ImageInfo imageInfo{ };
    imageInfo.physicalDevice = m_physicalDevice.get();
    imageInfo.device = m_device.get();
    imageInfo.extent = vk::Extent3D(m_swapchain.getExtent().width, m_swapchain.getExtent().height, 1);
    imageInfo.format = depthFormat;
    imageInfo.tiling = vk::ImageTiling::eOptimal;
    imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

    m_depthImage.init(imageInfo);

    // Image view for depth image
    ImageViewInfo depthImageViewInfo{
      .device = m_device.get(),
      .image = m_depthImage.get(),
      .format = depthFormat,
    };

    depthImageViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

    m_depthImageView.init(depthImageViewInfo);
  }

  void Api::initSwapchainFramebuffers()
  {
    FramebufferInfo framebufferInfo{
      .device = m_device.get(),
      .renderPass = m_renderPass.get(),
      .extent = m_swapchain.getExtent(),
      .depthImageView = m_depthImageView.get()
    };

    m_swapchainFramebuffers.resize(m_swapchainImageViews.size());
    for (size_t i = 0; i < m_swapchainFramebuffers.size(); ++i)
    {
      framebufferInfo.imageView = m_swapchainImageViews[i].get();
      m_swapchainFramebuffers[i].init(framebufferInfo);
    }
  }

  void Api::initDescriptorPool()
  {
    uint32_t swapchainImagesCount = m_swapchain.getImages().size();

    m_descriptorPool.init({
      .device = m_device.get(),
      .poolSizes = { { vk::DescriptorType::eUniformBuffer, swapchainImagesCount }, { vk::DescriptorType::eCombinedImageSampler, swapchainImagesCount } },
      .maxSets = maxNodes * swapchainImagesCount
      }
    );
  }

  void Api::initModel(const std::shared_ptr<GeometryNodeBase> node)
  {
    if (node->m_modelPath.empty())
      return;

    auto it = m_models.find(node->m_modelPath);
    RX_ASSERT((it != m_models.end()), "Can not upload uniform buffer because node does not have a (valid) model.");

    auto model = it->second;

    static auto queueIndices = m_queueManager.getUniqueQueueIndices({ GRAPHICS, TRANSFER });
    static auto queue = queueIndices.size() > 1 ? m_queueManager.getQueue(TRANSFER, queueIndices[1])->get() : m_queueManager.getQueue(GRAPHICS)->get();

    VertexBufferInfo vertexBufferInfo{
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .commandPool = m_transferCmdPool.get(),
      .queue = queue,
      .queueIndices = queueIndices.size() > 1 ? queueIndices : std::vector<uint32_t>()
    };

    IndexBufferInfo<uint32_t> indexBufferInfo{
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .commandPool = m_transferCmdPool.get(),
      .queue = queue,
      .queueIndices = queueIndices.size() > 1 ? queueIndices : std::vector<uint32_t>(),
    };

    if (!model->m_initialized)
    {
      vertexBufferInfo.vertices = model->m_vertices;
      model->m_vertexBuffer.init(vertexBufferInfo);

      indexBufferInfo.indices = model->m_indices;
      model->m_indexBuffer.init(indexBufferInfo);
      
      model->m_initialized = true;
    }

    node->m_uniformBuffers.init({
        .physicalDevice = m_physicalDevice.get(),
        .device = m_device.get(),
        .swapchainImagesCount = m_swapchain.getImages().size()
      }
    );

    SwapchainUpdateDescriptorSetInfo descriptorSetUpdateInfo{
      .descriptorPool = m_descriptorPool.get(),
      .uniformBuffers = node->m_uniformBuffers.getRaw()
    };

    // TODO: add support for multiple textures.
    auto diffuseIter = m_textures.find(node->m_material.m_diffuseTexture);
    if (diffuseIter != m_textures.end())
    {
      descriptorSetUpdateInfo.textureSampler = diffuseIter->second->getSampler();
      descriptorSetUpdateInfo.textureImageView = diffuseIter->second->getImageView();
    }

    // Create the descriptor set.
    model->m_descriptorSets.init({
        .device = m_device.get(),
        .pool = m_descriptorPool.get(),
        .setCount = static_cast<uint32_t>(m_swapchain.getImages().size()),
        .layouts = std::vector<vk::DescriptorSetLayout>(m_swapchain.getImages().size(), m_descriptorSetLayout.get()) // Create as many identical layouts as swapchain images exist.
      }
    );
    model->m_descriptorSets.update(descriptorSetUpdateInfo);
  }

  void Api::initModels(bool isNew)
  {
    static auto queueIndices = m_queueManager.getUniqueQueueIndices({ GRAPHICS, TRANSFER });
    static auto queue = queueIndices.size() > 1 ? m_queueManager.getQueue(TRANSFER, queueIndices[1])->get() : m_queueManager.getQueue(GRAPHICS)->get();

    VertexBufferInfo vertexBufferInfo{
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .commandPool = m_transferCmdPool.get(),
      .queue = queue,
      .queueIndices = queueIndices.size() > 1 ? queueIndices : std::vector<uint32_t>()
    };

    IndexBufferInfo<uint32_t> indexBufferInfo{
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .commandPool = m_transferCmdPool.get(),
      .queue = queue,
      .queueIndices = queueIndices.size() > 1 ? queueIndices : std::vector<uint32_t>()
    };

    UniformBufferInfo uniformBufferInfo{
      .physicalDevice = m_physicalDevice.get(),
      .device = m_device.get(),
      .swapchainImagesCount = m_swapchain.getImages().size()
    };

    for (const auto& node : m_nodes)
    {
      if (node->m_modelPath.empty())
        continue;

      auto it = m_models.find(node->m_modelPath);
      RX_ASSERT((it != m_models.end()), "Can not upload uniform buffer because node does not have a (valid) model.");

      auto model = it->second;

      if (model->m_initialized)
        continue;

      if (isNew)
      {
        vertexBufferInfo.vertices = model->m_vertices;
        model->m_vertexBuffer.init(vertexBufferInfo);

        indexBufferInfo.indices = model->m_indices;
        model->m_indexBuffer.init(indexBufferInfo);
      }

      node->m_uniformBuffers.init(uniformBufferInfo);

      SwapchainUpdateDescriptorSetInfo descriptorSetUpdateInfo{
        .descriptorPool = m_descriptorPool.get(),
        .uniformBuffers = node->m_uniformBuffers.getRaw()
      };

      // TODO: add support for multiple textures.
      auto diffuseIter = m_textures.find(node->m_material.m_diffuseTexture);
      if (diffuseIter != m_textures.end())
      {
        descriptorSetUpdateInfo.textureSampler = diffuseIter->second->getSampler();
        descriptorSetUpdateInfo.textureImageView = diffuseIter->second->getImageView();
      }

      // Create the descriptor set.
      model->m_descriptorSets.init({
          .device = m_device.get(),
          .pool = m_descriptorPool.get(),
          .setCount = static_cast<uint32_t>(m_swapchain.getImages().size()),
          .layouts = std::vector<vk::DescriptorSetLayout>(m_swapchain.getImages().size(), m_descriptorSetLayout.get())
        }
      );
      model->m_descriptorSets.update(descriptorSetUpdateInfo);

      model->m_initialized = true;
    }

    std::vector<std::shared_ptr<Model>> models;
    models.reserve(m_models.size());

    for (const auto& model : m_models)
      models.push_back(model.second);    

    m_raytraceBuilder.initAccelerationStructures(m_nodes, models);
  }

  void Api::initSwapchainCmdBuffers()
  {
    CmdBufferInfo commandBufferInfo{
      .device = m_device.get(),
      .commandPool = m_graphicsCmdPool.get(),
      .commandBufferCount = m_swapchainFramebuffers.size(),
      .usageFlags = vk::CommandBufferUsageFlagBits::eRenderPassContinue
    };

    m_swapchainCmdBuffers.init(commandBufferInfo);
  }

  void Api::initGui()
  {
    GuiInfo guiInfo{ };
    guiInfo.window = m_window->get();
    guiInfo.instance = m_instance.get();
    guiInfo.physicalDevice = m_physicalDevice.get();
    guiInfo.device = m_device.get();
    guiInfo.queueFamilyIndex = m_queueManager.getGraphicsFamilyIndex(); 
    guiInfo.queue = m_queueManager.getQueue(GRAPHICS)->get();
    guiInfo.minImageCount = m_surface.getCapabilities().minImageCount + 1;
    guiInfo.imageCount = static_cast<uint32_t>(m_swapchain.getImages().size());
    guiInfo.swapchainImageFormat = m_surface.getFormat();
    guiInfo.swapchainImageExtent = m_swapchain.getExtent();

    std::vector<vk::ImageView> temp(m_swapchainImageViews.size());
    
    for (size_t i = 0; i < m_swapchainImageViews.size(); ++i)
      temp[i] = m_swapchainImageViews[i].get();

    guiInfo.swapchainImageViews = temp;

    if (m_gui != nullptr)
      m_gui->init(guiInfo);
  }

  void Api::recordSwapchainCommandBuffers()
  {
    RX_LOG("Started recording.");

    /*
    std::unordered_map<std::string, uint32_t> models;
    for (const auto& node : m_nodes)
    {
      auto res = models.insert({ node->m_modelPath, 1 });
      if (!res.second)
      {
        auto it = models.find(node->m_modelPath);
        ++(it->second);
      }
    }
    */

    // Set up render pass begin info
    RenderPassBeginInfo renderPassBeginInfo{ };
    renderPassBeginInfo.renderArea = { 0, 0, m_swapchain.getExtent() };

    vk::ClearValue clearValues[2];
    clearValues[0].color = { std::array<float, 4>{ 0.5f, 0.5f, 0.5f, 1.0f } };
    clearValues[1].depthStencil = vk::ClearDepthStencilValue{ 1.0f, 0 };

    renderPassBeginInfo.clearValues = { clearValues[0], clearValues[1] };
    renderPassBeginInfo.commandBuffers = m_swapchainCmdBuffers.get();

    for (Framebuffer& framebuffer : m_swapchainFramebuffers)
      renderPassBeginInfo.framebuffers.push_back(framebuffer.get());

    m_renderPass.setBeginInfo(renderPassBeginInfo);

    // Start recording the swapchain framebuffers
    for (size_t imageIndex = 0; imageIndex < m_swapchainCmdBuffers.get().size(); ++imageIndex)
    {
      m_swapchainCmdBuffers.begin(imageIndex);
      m_renderPass.begin(imageIndex);

      m_swapchainCmdBuffers.get(imageIndex).bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline.get()); // CMD

      // Dynamic states
      vk::Viewport viewport = m_pipeline.getViewport();
      viewport.width = m_window->getProperties().getWidth();
      viewport.height = m_window->getProperties().getHeight();

      m_swapchainCmdBuffers.get(imageIndex).setViewport(0, 1, &viewport); // CMD

      vk::Rect2D scissor = m_pipeline.getScissor();
      scissor.extent = m_window->getExtent();

      m_swapchainCmdBuffers.get(imageIndex).setScissor(0, 1, &scissor); // CMD

      // Draw models
      for (const auto& node : m_nodes)
      {
        if (node->m_modelPath.empty())
          continue;

        auto it = m_models.find(node->m_modelPath);
        RX_ASSERT((it != m_models.end()), "Can not draw model because it was not found.");

        auto model = it->second;

        //auto it2 = models.find(model->m_path);
        //RX_ASSERT((it2 != models.end()), "Can not draw model because it was not found.");

        vk::Buffer vertexBuffers[] = { model->m_vertexBuffer.get() };
        vk::DeviceSize offsets[] = { 0 };

        m_swapchainCmdBuffers.get(imageIndex).bindVertexBuffers(
          0, // first binding
          1, // binding count
          vertexBuffers,
          offsets
        ); // CMD

        m_swapchainCmdBuffers.get(imageIndex).bindIndexBuffer(
          model->m_indexBuffer.get(),
          0, // offset
          model->m_indexBuffer.getType()
        ); // CMD

        m_swapchainCmdBuffers.get(imageIndex).bindDescriptorSets(
          vk::PipelineBindPoint::eGraphics, 
          m_pipeline.getLayout(),
          0, // first set
          1, // descriptor set count
          &model->m_descriptorSets.get()[imageIndex], // descriptor sets
          0, // dynamic offset count
          nullptr // dynamic offsets
        ); // CMD

        //std::cout << it2->first << ": " << it2->second << std::endl;

        m_swapchainCmdBuffers.get(imageIndex).drawIndexed(
          model->m_indexBuffer.getCount(), // index count
          1, //it2->second, // instance count
          0, // first index
          0, // vertex offset
          0 // first instance
        ); // CMD
      }

      m_renderPass.end(imageIndex);
      m_swapchainCmdBuffers.end(imageIndex);
    }
  }
}
