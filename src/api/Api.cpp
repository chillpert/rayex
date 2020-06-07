#include "Api.hpp"

namespace RX
{
  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  Api::Api(std::shared_ptr<Window> window) :
    m_window(window),
    m_gui(nullptr) { }

  Api::Api(std::shared_ptr<Window> window, std::unique_ptr<Gui> gui) :
    m_window(window),
    m_gui(std::move(gui)) { }

  Api::~Api()
  {
    clean();
  }

  void Api::initialize()
  {
    initInstance();
    initDebugMessenger();
    initSurface();
    initPhysicalDevice();
    initQueues();
    initRayTracing();
    initDevice();
    initRenderPass();
    initSwapchain();
    initSwapchainImageViews();
    initPipeline();
    initgraphicsCmdPool();
    inittransferCmdPool();
    initDepthBuffering();
    initSwapchainFramebuffers();
    initModels();
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
      m_imageAvailableSemaphores[i].initialize(semaphoreInfo);
      m_finishedRenderSemaphores[i].initialize(semaphoreInfo);
      m_inFlightFences[i].initialize(fenceInfo);
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
    if (m_window->changed())
    {
      recreateSwapchain();
      return true;
    }

    uint32_t imageIndex;
    m_swapchain.acquireNextImage(m_imageAvailableSemaphores[currentFrame].get(), nullptr, &imageIndex);

    // TODO: Temporary
    for (std::shared_ptr<Model> model : m_models)
    {
      UniformBufferObject ubo
      {
         model->m_model,
         model->m_view,
         model->m_projection,
         model->m_cameraPos
      };

      model->m_uniformBuffers.upload(imageIndex, ubo);
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
  
  void Api::clearModels()
  {
    m_models.clear();
  }

  void Api::pushModel(const std::shared_ptr<Model> model)
  {
    m_models.push_back(model);
  }

  void Api::setModels(const std::vector<std::shared_ptr<Model>>& models)
  {

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

    for (std::shared_ptr<Model> model : m_models)
      model->m_uniformBuffers.destroy();

    m_descriptorPool.destroy();

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
    initModels(false);
    initSwapchainCmdBuffers();
    recordSwapchainCommandBuffers();

    if (m_gui != nullptr)
    {
      m_gui->getInfo().minImageCount = m_surface.getCapabilities().minImageCount + 1;
      m_gui->getInfo().imageCount = static_cast<uint32_t>(m_swapchain.getImages().size());
      m_gui->getInfo().swapchainImageFormat = m_surface.getInfo().format;
      m_gui->getInfo().swapchainImageExtent = m_swapchain.getExtent();

      std::vector<vk::ImageView> temp(m_swapchainImageViews.size());

      for (size_t i = 0; i < m_swapchainImageViews.size(); ++i)
        temp[i] = m_swapchainImageViews[i].get();

      m_gui->getInfo().swapchainImageViews = temp;
      m_gui->recreate();
    }

    RX_LOG("Finished swapchain recreation.");
  }

  void Api::initInstance()
  {
    InstanceInfo instanceInfo{ };
    instanceInfo.window = m_window;
#ifdef RX_DEBUG
    instanceInfo.layers = { "VK_LAYER_KHRONOS_validation" };
#endif

#ifndef RX_TEST_BUILD
    instanceInfo.extensions = { "VK_KHR_get_physical_device_properties2" };
#endif

#ifdef RX_DEBUG
    if (instanceInfo.extensions.size() == 0)
      instanceInfo.extensions = { "VK_EXT_debug_utils" };
    else
      instanceInfo.extensions.push_back("VK_EXT_debug_utils");
#endif

    m_instance.initialize(instanceInfo);
  }

  void Api::initDebugMessenger()
  {
    DebugMessengerInfo debugMessengerInfo{ };
    debugMessengerInfo.instance = m_instance.get();

    m_debugMessenger.initialize(debugMessengerInfo);
  }

  void Api::initSurface()
  {
    SurfaceInfo surfaceInfo{ };
    surfaceInfo.window = m_window.get();
    surfaceInfo.instance = m_instance.get();

    m_surface.initialize(surfaceInfo);
  }

  void Api::initPhysicalDevice()
  {
    PhysicalDeviceInfo physicalDeviceInfo{ };
    physicalDeviceInfo.instance = m_instance.get();
    physicalDeviceInfo.surface = m_surface.get();

    m_physicalDevice.initialize(physicalDeviceInfo);

    // Reassess the support of the preferred surface settings.
    m_surface.checkSettingSupport(m_physicalDevice.get());
  }

  void Api::initQueues()
  {
    QueuesInfo queuesInfo{ };
    queuesInfo.physicalDevice = m_physicalDevice.get();
    queuesInfo.surface = m_surface.get();

    m_queueManager.initialize(queuesInfo);
    m_queueManager.print();
  }

  void Api::initDevice()
  {
    DeviceInfo deviceInfo{ };
    deviceInfo.physicalDevice = m_physicalDevice.get();
    deviceInfo.queueFamilies = m_queueManager.getQueueFamilies();

#ifndef RX_TEST_BUILD
    deviceInfo.extensions = { "VK_KHR_get_memory_requirements2", "VK_EXT_descriptor_indexing", "VK_KHR_buffer_device_address",  "VK_KHR_deferred_host_operations", "VK_KHR_pipeline_library", "VK_KHR_ray_tracing" };
#endif
    m_device.initialize(deviceInfo);

    // Retrieve all queue handles.
    m_queueManager.retrieveAllHandles(m_device.get());
  }

  void Api::initRenderPass()
  {
    RenderPassInfo renderPassInfo{ };
    renderPassInfo.physicalDevice = m_physicalDevice.get();
    renderPassInfo.device = m_device.get();
    renderPassInfo.surfaceFormat = m_surface.getInfo().format;
    renderPassInfo.depthFormat = getSupportedDepthFormat(m_physicalDevice.get());
    
    if (m_gui == nullptr)
      renderPassInfo.guiEnabled = false;
    else
      renderPassInfo.guiEnabled = true;

    m_renderPass.initialize(renderPassInfo);
  }

  void Api::initSwapchain()
  {
    m_surface.checkSettingSupport(m_physicalDevice.get());

    SwapchainInfo swapchainInfo{ };
    swapchainInfo.window = m_window.get();
    swapchainInfo.physicalDevice = m_physicalDevice.get();
    swapchainInfo.device = m_device.get();
    swapchainInfo.surface = &m_surface;
    swapchainInfo.queueFamilyIndices = m_queueManager.getQueueFamilyIndicesForSwapchainAccess();
    swapchainInfo.imageAspect = vk::ImageAspectFlagBits::eColor;
    swapchainInfo.renderPass = m_renderPass.get();

    m_swapchain.initialize(swapchainInfo);
  }

  void Api::initSwapchainImageViews()
  {
    m_swapchainImageViews.resize(m_swapchain.getImages().size());
    for (size_t i = 0; i < m_swapchainImageViews.size(); ++i)
    {
      ImageViewInfo imageViewInfo{ };
      imageViewInfo.device = m_device.get();
      imageViewInfo.image = m_swapchain.getImage(i);
      imageViewInfo.format = m_surface.getInfo().format;
      imageViewInfo.subresourceRange.aspectMask = m_swapchain.getInfo().imageAspect;

      m_swapchainImageViews[i].initialize(imageViewInfo);
    }
  }

  void Api::initPipeline(bool firstRun)
  {
    // Shaders
    Shader vs;
    ShaderInfo vertexShaderInfo{ };
    vertexShaderInfo.fullPath = RX_SHADER_PATH "simple3D.vert";
    vertexShaderInfo.device = m_device.get();
    vertexShaderInfo.binding = 0;
    vertexShaderInfo.descriptorCount = 1;
    vertexShaderInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
    vertexShaderInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vs.initialize(vertexShaderInfo);

    Shader fs;
    ShaderInfo fragmentShaderInfo{ };
    fragmentShaderInfo.fullPath = RX_SHADER_PATH "simple3D.frag";
    fragmentShaderInfo.device = m_device.get();
    fragmentShaderInfo.binding = 1;
    fragmentShaderInfo.descriptorCount = 1;
    fragmentShaderInfo.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    fragmentShaderInfo.stageFlags = vk::ShaderStageFlagBits::eFragment;

    fs.initialize(fragmentShaderInfo);

    if (firstRun)
    {
      // Descriptor Set Layout
      DescriptorSetLayoutInfo descriptorSetLayoutInfo{ };
      descriptorSetLayoutInfo.device = m_device.get();
      descriptorSetLayoutInfo.layoutBindings = { vs.getDescriptorSetLayoutBinding(), fs.getDescriptorSetLayoutBinding() };

      m_descriptorSetLayout.initialize(descriptorSetLayoutInfo);
    }

    // Graphics pipeline
    PipelineInfo pipelineInfo{ };
    pipelineInfo.device = m_device.get();
    pipelineInfo.renderPass = m_renderPass.get();
    pipelineInfo.viewport = vk::Viewport{ 0.0f, 0.0f, static_cast<float>(m_swapchain.getExtent().width), static_cast<float>(m_swapchain.getExtent().height), 0.0f, 1.0f };
    pipelineInfo.scissor = { 0, 0, m_swapchain.getExtent().width, m_swapchain.getExtent().height };
    pipelineInfo.vertexShader = vs.get();
    pipelineInfo.fragmentShader = fs.get();
    pipelineInfo.descriptorSetLayout = m_descriptorSetLayout.get();

    m_pipeline.initialize(pipelineInfo);
  }

  void Api::initgraphicsCmdPool()
  {
    CommandPoolInfo commandPoolInfo{ };
    commandPoolInfo.device = m_device.get();
    commandPoolInfo.queueFamilyIndex = m_queueManager.getGraphicsFamilyIndex();

    m_graphicsCmdPool.initialize(commandPoolInfo);
  }

  void Api::inittransferCmdPool()
  {
    CommandPoolInfo commandPoolInfo{ };
    commandPoolInfo.device = m_device.get();
    // With the given parameters the function will return two different queue family indices. 
    // This means that the transfer queue family index will be a different one than the graphics queue family index.
    // However, this will only be the case if the queue family indices on this device allow it.
    static auto indices = m_queueManager.getUniqueQueueIndices({ GRAPHICS, TRANSFER });
    commandPoolInfo.queueFamilyIndex = indices.size() > 1 ? indices[1] : m_queueManager.getTransferFamilyIndex();

    m_transferCmdPool.initialize(commandPoolInfo);
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

    m_depthImage.initialize(imageInfo);

    // Image view for depth image
    ImageViewInfo depthImageViewInfo{ };
    depthImageViewInfo.device = m_device.get();
    depthImageViewInfo.format = depthFormat;
    depthImageViewInfo.image = m_depthImage.get();
    depthImageViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

    m_depthImageView.initialize(depthImageViewInfo);
  }

  void Api::initSwapchainFramebuffers()
  {
    FramebufferInfo framebufferInfo{ };
    framebufferInfo.device = m_device.get();
    framebufferInfo.depthImageView = m_depthImageView.get();
    framebufferInfo.renderPass = m_renderPass.get();
    framebufferInfo.extent = m_swapchain.getExtent();

    m_swapchainFramebuffers.resize(m_swapchainImageViews.size());
    for (size_t i = 0; i < m_swapchainFramebuffers.size(); ++i)
    {
      framebufferInfo.imageView = m_swapchainImageViews[i].get();
      m_swapchainFramebuffers[i].initialize(framebufferInfo);
    }
  }

  void Api::initModels(bool firstRun)
  {
    TextureInfo textureInfo{ };
    textureInfo.physicalDevice = m_physicalDevice.get();
    textureInfo.device = m_device.get();
    textureInfo.commandPool = m_graphicsCmdPool.get();
    textureInfo.queue = m_queueManager.getGraphicsQueue();

    static auto queueIndices = m_queueManager.getUniqueQueueIndices({ GRAPHICS, TRANSFER });
    static auto queue = queueIndices.size() > 1 ? m_queueManager.getTransferQueue(queueIndices[1]) : m_queueManager.getGraphicsQueue();

    VertexBufferInfo vertexBufferInfo{ };
    vertexBufferInfo.physicalDevice = m_physicalDevice.get();
    vertexBufferInfo.device = m_device.get();
    vertexBufferInfo.commandPool = m_transferCmdPool.get();
    vertexBufferInfo.queue = queue;
    vertexBufferInfo.queueIndices = queueIndices.size() > 1 ? queueIndices : std::vector<uint32_t>();

    IndexBufferInfo indexBufferInfo{ };
    indexBufferInfo.physicalDevice = m_physicalDevice.get();
    indexBufferInfo.device = m_device.get();
    indexBufferInfo.commandPool = m_transferCmdPool.get();
    indexBufferInfo.queue = queue;
    indexBufferInfo.queueIndices = queueIndices.size() > 1 ? queueIndices : std::vector<uint32_t>();

    UniformBufferInfo uniformBufferInfo{ };
    uniformBufferInfo.physicalDevice = m_physicalDevice.get();
    uniformBufferInfo.device = m_device.get();
    uniformBufferInfo.swapchainImagesCount = m_swapchain.getImages().size();

    DescriptorPoolInfo descriptorPoolInfo{ };
    descriptorPoolInfo.device = m_device.get();
    uint32_t swapchainImagesCount = m_swapchain.getImages().size();
    descriptorPoolInfo.poolSizes = { { vk::DescriptorType::eUniformBuffer, swapchainImagesCount }, { vk::DescriptorType::eCombinedImageSampler, swapchainImagesCount } };
    descriptorPoolInfo.maxSets = m_models.size() * swapchainImagesCount;

    m_descriptorPool.initialize(descriptorPoolInfo);

    DescriptorSetInfo descriptorSetInfo{ };
    descriptorSetInfo.device = m_device.get();
    descriptorSetInfo.swapchainImagesCount = m_swapchain.getImages().size();
    descriptorSetInfo.descriptorSetLayout = m_descriptorSetLayout.get();

    if (firstRun)
    {
      for (std::shared_ptr<Model> model : m_models)
      {
        model->initialize();

        textureInfo.path = model->m_pathToTexture;
        if (!textureInfo.path.empty())
          model->m_texture.initialize(textureInfo);

        vertexBufferInfo.vertices = model->m_vertices;
        model->m_vertexBuffer.initialize(vertexBufferInfo);

        indexBufferInfo.indices = model->m_indices;
        model->m_indexBuffer.initialize(indexBufferInfo);

        uniformBufferInfo.uniformBufferObject = model->getUbo();
        model->m_uniformBuffers.initialize(uniformBufferInfo);

        descriptorSetInfo.descriptorPool = m_descriptorPool.get();
        descriptorSetInfo.textureSampler = model->m_texture.getSampler();
        descriptorSetInfo.textureImageView = model->m_texture.getImageView();
        descriptorSetInfo.uniformBuffers = model->m_uniformBuffers.getRaw();

        model->m_descriptorSets.initialize(descriptorSetInfo);
      }
    }
    else
    {
      for (std::shared_ptr<Model> model : m_models)
      {
        uniformBufferInfo.uniformBufferObject = model->getUbo();
        model->m_uniformBuffers.initialize(uniformBufferInfo);

        descriptorSetInfo.descriptorPool = m_descriptorPool.get();
        descriptorSetInfo.textureSampler = model->m_texture.getSampler();
        descriptorSetInfo.textureImageView = model->m_texture.getImageView();
        descriptorSetInfo.uniformBuffers = model->m_uniformBuffers.getRaw();

        model->m_descriptorSets.initialize(descriptorSetInfo);
      }
    }
  }

  void Api::initSwapchainCmdBuffers()
  {
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_device.get();
    commandBufferInfo.commandPool = m_graphicsCmdPool.get();
    commandBufferInfo.commandBufferCount = m_swapchainFramebuffers.size();
    commandBufferInfo.usageFlags = vk::CommandBufferUsageFlagBits::eRenderPassContinue;
    commandBufferInfo.freeAutomatically = true;
    commandBufferInfo.componentName = "swapchain command buffers";

    m_swapchainCmdBuffers.initialize(commandBufferInfo);
  }

  void Api::initGui()
  {
    GuiInfo guiInfo{ };
    guiInfo.window = m_window->get();
    guiInfo.instance = m_instance.get();
    guiInfo.physicalDevice = m_physicalDevice.get();
    guiInfo.device = m_device.get();
    guiInfo.queueFamilyIndex = m_queueManager.getGraphicsFamilyIndex(); 
    guiInfo.queue = m_queueManager.getGraphicsQueue();
    guiInfo.minImageCount = m_surface.getCapabilities().minImageCount + 1;
    guiInfo.imageCount = static_cast<uint32_t>(m_swapchain.getImages().size());
    guiInfo.swapchainImageFormat = m_surface.getInfo().format;
    guiInfo.swapchainImageExtent = m_swapchain.getExtent();

    std::vector<vk::ImageView> temp(m_swapchainImageViews.size());
    
    for (size_t i = 0; i < m_swapchainImageViews.size(); ++i)
      temp[i] = m_swapchainImageViews[i].get();

    guiInfo.swapchainImageViews = temp;

    if (m_gui != nullptr)
      m_gui->initialize(guiInfo);
  }

  void Api::recordSwapchainCommandBuffers()
  {
    RX_LOG("Started recording.");

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

      vkCmdBindPipeline(m_swapchainCmdBuffers.get()[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.get());

      // Dynamic states
      vk::Viewport viewport = m_pipeline.getInfo().viewport;
      viewport.width = m_window->getProperties().getWidth();
      viewport.height = m_window->getProperties().getHeight();

      m_swapchainCmdBuffers.get()[imageIndex].setViewport(0, 1, &viewport); // CMD

      vk::Rect2D scissor = m_pipeline.getInfo().scissor;
      scissor.extent = m_window->getExtent();

      m_swapchainCmdBuffers.get()[imageIndex].setScissor(0, 1, &scissor); // CMD

      // Draw models
      for (std::shared_ptr<Model> model : m_models)
      {
        vk::Buffer vertexBuffers[] = { model->m_vertexBuffer.get() };
        vk::DeviceSize offsets[] = { 0 };

        m_swapchainCmdBuffers.get()[imageIndex].bindVertexBuffers(0, 1, vertexBuffers, offsets); // CMD
        m_swapchainCmdBuffers.get()[imageIndex].bindIndexBuffer(model->m_indexBuffer.get(), 0, model->m_indexBuffer.getType()); // CMD
        m_swapchainCmdBuffers.get()[imageIndex].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipeline.getLayout(), 0, 1, &model->m_descriptorSets.get()[imageIndex], 0, nullptr); // CMD
        m_swapchainCmdBuffers.get()[imageIndex].drawIndexed(model->m_indexBuffer.getCount(), 1, 0, 0, 0); // CMD
      }

      m_renderPass.end(imageIndex);
      m_swapchainCmdBuffers.end(imageIndex);
    }
  }
}

namespace RX
{
  void Api::initRayTracing()
  {
    auto properties = m_physicalDevice.get().getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesKHR>();
    m_rayTracingProperties = properties.get<vk::PhysicalDeviceRayTracingPropertiesKHR>();
  }
}

