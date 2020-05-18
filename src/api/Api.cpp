#include "api/Api.hpp"

namespace RX
{
  Api::Api(std::shared_ptr<Window> window) :
    m_window(window) { }

  void Api::initialize()
  {
    // Required for extending the physical m_device from m_device extensions.
    m_instance.pushExtension("VK_KHR_get_physical_device_properties2");
    m_instance.initialize(m_window);
    //m_instance.print();

    m_debugMessenger.initialize(m_instance.get());

    m_surface.initialize(m_instance.get(), m_window);

    m_physicalDevice.initialize(m_instance.get(), m_surface.get());

    // Extensions required for ray tracing.
    std::vector<const char*> requiredExtensions =
    {
      "VK_KHR_get_memory_requirements2",
      "VK_EXT_descriptor_indexing",
      "VK_KHR_buffer_device_address",
      "VK_KHR_deferred_host_operations",
      "VK_KHR_pipeline_library",
      "VK_KHR_ray_tracing"
    };

    m_physicalDevice.checkExtensionSupport(requiredExtensions);

    // Set up queues.
    m_queueManager.initialize(m_physicalDevice.get(), m_surface.get());

    // Add all of the device extensions from above.
    for (const auto& extension : requiredExtensions)
      m_device.pushExtension(extension);

    m_device.initialize(m_physicalDevice.get(), m_queueManager);

    m_renderPass.initialize(m_device.get(), m_surface.getFormat(m_physicalDevice.get()).format);

    m_swapchain.initialize(m_physicalDevice.get(), m_device.get(), m_surface, m_window, m_queueManager);
    m_images.initialize(m_device.get(), m_swapchain.get());
    m_imageViews.initialize(m_device.get(), m_surface.getFormat().format, m_images);
    m_framebuffers.initialize(m_device.get(), m_imageViews, m_renderPass.get(), m_window);
    
    Shader vs, fs;
    vs.initialize(RX_SHADER_PATH "test.vert", m_device.get());
    fs.initialize(RX_SHADER_PATH "test.frag", m_device.get());
    m_pipeline.initialize(m_device.get(), m_renderPass.get(), m_swapchain.getExtent(), m_window, vs, fs);
    
    m_commandPool.initialize(m_device.get(), m_queueManager.getGraphicsIndex());
    m_commandBuffers.initialize(m_device.get(), m_commandPool.get(), m_framebuffers.get().size());
    m_commandBuffers.record(m_swapchain, m_framebuffers, m_renderPass, m_pipeline);

    m_imageAvailableSemaphore.initialize(m_device.get());
    m_finishedRenderSemaphore.initialize(m_device.get());
  }

  bool Api::update()
  {
    return true;
  }

  bool Api::render()
  {
    uint32_t imageIndex;
    VK_ASSERT(vkAcquireNextImageKHR(m_device.get(), m_swapchain.get(), UINT64_MAX, m_imageAvailableSemaphore.get(), VK_NULL_HANDLE, &imageIndex), "Failed to acquire image from swapchain");

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore.get() };
    VkSemaphore signalSemaphores[] = { m_finishedRenderSemaphore.get() };

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo{ };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    // Define which command buffers should be submitted for execution.
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers.get()[imageIndex];
    // Define which semaphores to signal once the command buffers have finished execution.
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    m_queueManager.submit(submitInfo);

    VkPresentInfoKHR presentInfo{ };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapchains[] = { m_swapchain.get() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;

    m_queueManager.present(presentInfo);

    return true;
  }

  void Api::clean()
  {
    vkDeviceWaitIdle(m_device.get());
  }
}