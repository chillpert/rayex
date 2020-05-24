#include "Api.hpp"
#include "buffers/Vertex.hpp"

namespace RX
{
  // Defines the maximum amount of frames that will be processed concurrently.
  const size_t maxFramesInFlight = 2;

  Api::Api(std::shared_ptr<Window> window) :
    m_window(window) { }

  void Api::initialize()
  {
    // This extension is required for extending the physical m_device from m_device extensions.
    // A list of available layers and instance extensions can be printed by using Instance::print. 
    m_instance.pushExtension("VK_KHR_get_physical_device_properties2");
    // Set up the instance for interacting with the Vulkan library.
    m_instance.initialize(m_window);

    // Set up debug messenger. This will only have an effect if the build is a release build.
    m_debugMessenger.initialize(m_instance.get());

    // Set up the window surface that will access the actual SDL window.
    m_surface.initialize(m_instance.get(), m_window);

    // Enumerate and pick one of the available physical devices on the given device.
    m_physicalDevice.initialize(m_instance.get(), m_surface.get());

    // All device extensions required for ray tracing.
    std::vector<const char*> requiredExtensions =
    {
      "VK_KHR_get_memory_requirements2",
      "VK_EXT_descriptor_indexing",
      "VK_KHR_buffer_device_address",
      "VK_KHR_deferred_host_operations",
      "VK_KHR_pipeline_library",
      "VK_KHR_ray_tracing"
    };

    // Make sure that the device extensions are supported by the physical device.
    m_physicalDevice.checkExtensionSupport(requiredExtensions);

    // Set up queues.
    m_queues.initialize(m_physicalDevice.get(), m_surface.get());

    // Add all of the device extensions from above.
    for (const auto& extension : requiredExtensions)
      m_device.pushExtension(extension);

    // Set up the logical device.
    m_device.initialize(m_physicalDevice.get(), m_queues);

    // Retrieve all queue handles.
    m_queues.retrieveAllHandles(m_device.get());

    // Set up the render pass.
    m_renderPass.initialize(m_device.get(), m_surface.getFormat(m_physicalDevice.get()).format);

    // Set up the swapchain and its related components.
    m_swapchain.initialize(m_physicalDevice.get(), m_device.get(), m_surface, m_window, m_queues);
    m_images.initialize(m_device.get(), m_swapchain.get());
    m_imageViews.initialize(m_device.get(), m_surface.getFormat().format, m_images);
    m_framebuffers.initialize(m_device.get(), m_imageViews, m_renderPass.get(), m_window);
    
    // Set up simple example shaders.
    Shader vs, fs;
    vs.initialize(RX_SHADER_PATH "simple3D.vert", m_device.get());
    fs.initialize(RX_SHADER_PATH "simple3D.frag", m_device.get());
    
    m_descriptorSetLayout.initialize(m_device.get());

    // Set up the graphics pipeline.
    m_pipeline.initialize(m_device.get(), m_renderPass.get(), m_swapchain.getExtent(), m_window, vs, fs, m_descriptorSetLayout.get());
    
    // Set up the command pool, allocate the command buffer and start command buffer recording.
    m_graphicsCmdPool.initialize(m_device.get(), m_queues.getGraphicsIndex()); // TODO: What if the graphics and present index are not identical?

    // TODO: temporary
    
    m_texture.initialize(m_physicalDevice.get(), m_device.get(), m_queues.getGraphicsQueue(), m_graphicsCmdPool.get(), RX_TEXTURE_PATH "awesomeface.png");

    for (auto model : m_models)
    {
      m_vertexBuffer.initialize(m_device.get(), m_physicalDevice.get(), m_graphicsCmdPool.get(), m_queues.getGraphicsQueue(), model->vertices);
      m_indexBuffer.initialize<uint32_t>(m_device.get(), m_physicalDevice.get(), m_graphicsCmdPool.get(), m_queues.getGraphicsQueue(), model->indices); 
      m_uniformBuffers.initialize(m_device.get(), m_physicalDevice.get(), m_swapchain.getExtent(), m_images.getSize(), model->ubo);
    }

    m_descriptorPool.initialize(m_device.get(), m_images.getSize());
    m_descriptorSets.initialize(m_device.get(), m_images.getSize(), m_descriptorPool.get(), m_descriptorSetLayout.get(), m_uniformBuffers.get(), m_texture);

    m_commandBuffers.initialize(m_device.get(), m_graphicsCmdPool.get(), m_framebuffers.getSize());
    m_commandBuffers.record(m_swapchain, m_framebuffers, m_renderPass, m_pipeline, m_vertexBuffer, m_indexBuffer, m_descriptorSets);

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

    uint32_t imageIndex;
    m_swapchain.acquireNextImage(m_device.get(), m_imageAvailableSemaphores[currentFrame].get(), VK_NULL_HANDLE, &imageIndex);

    // TODO: Temporary
    m_uniformBuffers.upload(imageIndex);

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
    m_framebuffers.destroy();
    m_commandBuffers.destroy();
    m_pipeline.destroy();
    m_renderPass.destroy();
    m_imageViews.destroy();
    m_swapchain.destroy();
    m_uniformBuffers.destroy();
    m_descriptorPool.destroy();

    // 2. Recreating the swapchain.
    m_swapchain.initialize(m_physicalDevice.get(), m_device.get(), m_surface, m_window, m_queues);
    m_images.initialize(m_device.get(), m_swapchain.get());
    m_imageViews.initialize(m_device.get(), m_surface.getFormat().format, m_images);
    m_renderPass.initialize(m_device.get(), m_surface.getFormat(m_physicalDevice.get()).format);
    
    Shader vs, fs;
    vs.initialize(RX_SHADER_PATH "simple3D.vert", m_device.get());
    fs.initialize(RX_SHADER_PATH "simple3D.frag", m_device.get());
    m_pipeline.initialize(m_device.get(), m_renderPass.get(), m_swapchain.getExtent(), m_window, vs, fs, m_descriptorSetLayout.get());

    m_framebuffers.initialize(m_device.get(), m_imageViews, m_renderPass.get(), m_window);

    for (auto model : m_models)
    {
      m_uniformBuffers.initialize(m_device.get(), m_physicalDevice.get(), m_swapchain.getExtent(), m_images.getSize(), model->ubo);
    }

    m_descriptorPool.initialize(m_device.get(), m_images.getSize());
    m_descriptorSets.initialize(m_device.get(), m_images.getSize(), m_descriptorPool.get(), m_descriptorSetLayout.get(), m_uniformBuffers.get(), m_texture);
    m_commandBuffers.initialize(m_device.get(), m_graphicsCmdPool.get(), m_framebuffers.getSize());
    m_commandBuffers.record(m_swapchain, m_framebuffers, m_renderPass, m_pipeline, m_vertexBuffer, m_indexBuffer, m_descriptorSets);

    RX_ENABLE_LOG;
  }
}