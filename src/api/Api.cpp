#include "Api.hpp"
#include "buffers/Vertex.hpp"

namespace RX
{
  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  RenderPassInfo renderPassInfo{ };
  SwapchainInfo swapchainInfo{ };

  Api::Api(std::shared_ptr<Window> window) :
    m_window(window) { }

  Api::~Api()
  {
    clean();
  }

  void Api::initialize()
  {
    // Instance
    InstanceInfo instanceInfo{ };
    instanceInfo.window = m_window;
    instanceInfo.layers = { "VK_LAYER_KHRONOS_validation" };
    instanceInfo.extensions = { "VK_EXT_debug_utils", "VK_KHR_get_physical_device_properties2" };

    m_instance.initialize(instanceInfo);

    // Debug messenger
    DebugMessengerInfo debugMessengerInfo{ };
    debugMessengerInfo.instance = m_instance.get();

    m_debugMessenger.initialize(debugMessengerInfo);

    // Surface
    SurfaceInfo surfaceInfo{ };
    surfaceInfo.window = m_window;
    surfaceInfo.instance = m_instance.get();

    m_surface.initialize(surfaceInfo);

    // Physical device
    PhysicalDeviceInfo physicalDeviceInfo{ };
    physicalDeviceInfo.instance = m_instance.get();
    physicalDeviceInfo.surface = m_surface.get();
    
    m_physicalDevice.initialize(physicalDeviceInfo);

    // Reassess the support of the preferred surface settings.
    m_surface.checkSettingSupport(m_physicalDevice.get());

    // Queues
    QueuesInfo queuesInfo{ };
    queuesInfo.physicalDevice = m_physicalDevice.get();
    queuesInfo.surface = m_surface.get();

    m_queues.initialize(queuesInfo);

    // Device
    DeviceInfo deviceInfo{ };
    deviceInfo.physicalDevice = m_physicalDevice.get();
    deviceInfo.queueFamilyIndices = m_queues.getQueueFamilyIndices();
    deviceInfo.extensions = { "VK_KHR_get_memory_requirements2", "VK_EXT_descriptor_indexing", "VK_KHR_buffer_device_address",  "VK_KHR_deferred_host_operations", "VK_KHR_pipeline_library", "VK_KHR_ray_tracing"};

    m_device.initialize(deviceInfo);

    // Retrieve all queue handles.
    m_queues.retrieveAllHandles(m_device.get());

    // Render pass
    renderPassInfo.physicalDevice = m_physicalDevice.get();
    renderPassInfo.device = m_device.get();
    renderPassInfo.surfaceFormat = m_surface.getInfo().format;
    renderPassInfo.depthFormat = DepthImage::getSupportedFormat(m_physicalDevice.get());

    m_renderPass.initialize(renderPassInfo);

    // Swapchain
    swapchainInfo.window = m_window;
    swapchainInfo.physicalDevice = m_physicalDevice.get();
    swapchainInfo.device = m_device.get();
    swapchainInfo.surface = m_surface.get();
    swapchainInfo.surfaceFormat = m_surface.getInfo().format;
    swapchainInfo.surfaceColorSpace = m_surface.getInfo().colorSpace;
    swapchainInfo.surfacePresentMode = m_surface.getInfo().presentMode;
    swapchainInfo.surfaceCapabilities = m_surface.getCapabilitites(m_physicalDevice.get());
    swapchainInfo.queueFamilyIndices = m_queues.getQueueFamilyIndices();

    m_swapchain.initialize(swapchainInfo);

    // Images
    m_images.initialize(m_device.get(), m_swapchain.get());
    m_imageViews.initialize(m_device.get(), m_surface.getInfo().format, m_images);

    // Set up simple example shaders.
    Shader vs, fs;
    vs.initialize(RX_SHADER_PATH "simple3D.vert", m_device.get());
    fs.initialize(RX_SHADER_PATH "simple3D.frag", m_device.get());
    
    m_descriptorSetLayout.initialize(m_device.get());
    
    // Set up the graphics pipeline.
    m_pipeline.initialize(m_device.get(), m_renderPass.get(), m_swapchain.getInfo().extent, m_window, vs, fs, m_descriptorSetLayout.get());
    
    // Set up the command pool, allocate the command buffer and start command buffer recording.
    m_graphicsCmdPool.initialize(m_device.get(), m_queues.getGraphicsFamilyIndex()); // TODO: What if the graphics and present index are not identical?

    m_depthImage.initialize(m_physicalDevice.get(), m_device.get(), m_swapchain.getInfo().extent);
    m_framebuffers.initialize(m_device.get(), m_imageViews, m_depthImage.getView(), m_renderPass.get(), m_window);

    m_descriptorPool.initialize(m_device.get(), m_images.getSize());
    
    m_imgui.initialize(
      m_instance.get(),
      m_physicalDevice.get(),
      m_device.get(),
      m_queues,
      m_descriptorPool.get(),
      m_surface,
      m_renderPass.get(),
      m_window->get(),
      m_images.getSize()
    );

    for (std::shared_ptr<Model> model : m_models)
    {
      model->initialize();

      model->m_texture.initialize(m_physicalDevice.get(), m_device.get(), m_queues.getGraphicsQueue(), m_graphicsCmdPool.get(), model->m_pathToTexture);
      model->m_vertexBuffer.initialize(m_device.get(), m_physicalDevice.get(), m_graphicsCmdPool.get(), m_queues.getGraphicsQueue(), model->m_vertices);
      model->m_indexBuffer.initialize<uint32_t>(m_device.get(), m_physicalDevice.get(), m_graphicsCmdPool.get(), m_queues.getGraphicsQueue(), model->m_indices);
      model->m_uniformBuffers.initialize(m_device.get(), m_physicalDevice.get(), m_swapchain.getInfo().extent, m_images.getSize(), model->getUbo());

      model->m_descriptorPool.initialize(m_device.get(), m_images.getSize());
      model->m_descriptorSets.initialize(m_device.get(), m_images.getSize(), model->m_descriptorPool.get(), m_descriptorSetLayout.get(), model->m_uniformBuffers.get(), model->m_texture);
    }

    m_commandBuffers.initialize(m_device.get(), m_graphicsCmdPool.get(), m_framebuffers.getSize());
    m_commandBuffers.record(m_swapchain, m_framebuffers, m_renderPass, m_pipeline, m_models);

    // TODO: transfer command pool inizialization and recording

    // Set up the synchronization objects.
    m_imageAvailableSemaphores.resize(maxFramesInFlight);
    m_finishedRenderSemaphores.resize(maxFramesInFlight);
    m_inFlightFences.resize(maxFramesInFlight);
    m_imagesInFlight.resize(m_images.getSize(), VK_NULL_HANDLE);

    for (size_t i = 0; i < maxFramesInFlight; ++i)
    {
      m_imageAvailableSemaphores[i].initialize(m_device.get());
      m_finishedRenderSemaphores[i].initialize(m_device.get());
      m_inFlightFences[i].initialize(m_device.get());
    }
  }

  bool Api::update()
  {
    return true;
  }

  bool Api::render()
  {
    static size_t currentFrame = 0;

    // Wait for the current frame's fences.
    vkWaitForFences(m_device.get(), 1, &m_inFlightFences[currentFrame].get(), VK_TRUE, UINT64_MAX);

    // If the window is minimized then simply do not render anything anymore.
    if (m_window->minimized())
      return true;

    // If the window size has changed the swapchain has to be recreated.
    if (m_window->changed())
    {
      recreateSwapchain();
      return true;
    }

    m_imgui.beginRender();
    m_imgui.renderDemo();
    m_imgui.render();

    uint32_t imageIndex;
    m_swapchain.acquireNextImage(m_device.get(), m_imageAvailableSemaphores[currentFrame].get(), VK_NULL_HANDLE, &imageIndex);

    // TODO: Temporary
    for (std::shared_ptr<Model> model : m_models)
    {
      model->m_uniformBuffers.upload(imageIndex, model->getUbo());
    }

    // Check if a previous frame is using the current image.
    if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
      vkWaitForFences(m_device.get(), 1, &m_imagesInFlight[currentFrame], VK_TRUE, UINT64_MAX);

    // This will mark the current image to be in use by this frame.
    m_imagesInFlight[imageIndex] = m_inFlightFences[currentFrame].get();

    VkSubmitInfo submitInfo{ };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[currentFrame].get() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers.get()[imageIndex];

    VkSemaphore signalSemaphores[] = { m_finishedRenderSemaphores[currentFrame].get() };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    m_imgui.endRender(m_device.get(), m_queues, m_renderPass.get(), m_framebuffers.get()[imageIndex]);

    // Reset the signaled state of the current frame's fence to the unsignaled one.
    vkResetFences(m_device.get(), 1, &m_inFlightFences[currentFrame].get());

    // Submits / executes the current image's / framebuffer's command buffer.
    m_queues.submit(submitInfo, m_inFlightFences[currentFrame].get());

    VkPresentInfoKHR presentInfo{ };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapchain.get() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    // Tell the presentation engine that the current image is ready.
    m_queues.present(presentInfo);

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
    VK_LOG("Recreating swapchain");
    RX_DISABLE_LOG;

    m_device.waitIdle();

    // 1. Cleaning the existing swapchain.
    m_depthImage.destroy();
    m_framebuffers.destroy();
    m_commandBuffers.destroy();
    m_pipeline.destroy();
    m_renderPass.destroy();
    m_imageViews.destroy();
    m_swapchain.destroy();

    for (std::shared_ptr<Model> model : m_models)
      model->m_uniformBuffers.destroy();

    m_descriptorPool.destroy();

    // 2. Recreating the swapchain.
    m_swapchain.initialize(swapchainInfo);
    m_images.initialize(m_device.get(), m_swapchain.get());
    m_imageViews.initialize(m_device.get(), m_surface.getInfo().format, m_images);
    m_renderPass.initialize(renderPassInfo);

    Shader vs, fs;
    vs.initialize(RX_SHADER_PATH "simple3D.vert", m_device.get());
    fs.initialize(RX_SHADER_PATH "simple3D.frag", m_device.get());
    m_pipeline.initialize(m_device.get(), m_renderPass.get(), m_swapchain.getInfo().extent, m_window, vs, fs, m_descriptorSetLayout.get());

    m_depthImage.initialize(m_physicalDevice.get(), m_device.get(), m_swapchain.getInfo().extent);
    m_framebuffers.initialize(m_device.get(), m_imageViews, m_depthImage.getView(), m_renderPass.get(), m_window);

    m_descriptorPool.initialize(m_device.get(), m_images.getSize());

    for (auto model : m_models)
    {
      model->m_uniformBuffers.initialize(m_device.get(), m_physicalDevice.get(), m_swapchain.getInfo().extent, m_images.getSize(), model->getUbo());
      model->m_descriptorPool.initialize(m_device.get(), m_images.getSize());
      model->m_descriptorSets.initialize(m_device.get(), m_images.getSize(), model->m_descriptorPool.get(), m_descriptorSetLayout.get(), model->m_uniformBuffers.get(), model->m_texture);
    }

    m_commandBuffers.initialize(m_device.get(), m_graphicsCmdPool.get(), m_framebuffers.getSize());
    m_commandBuffers.record(m_swapchain, m_framebuffers, m_renderPass, m_pipeline, m_models);

    RX_ENABLE_LOG;
  }
}