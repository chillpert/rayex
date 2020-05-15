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
#ifdef RX_DEBUG
    m_instance.print();
#endif
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
    //for (const auto& extension : requiredExtensions)
    //  m_device.pushExtension(extension);

    m_device.initialize(m_physicalDevice.get(), m_queueManager);

    m_swapchain.initialize(m_physicalDevice.get(), m_device.get(), m_surface, m_window, m_queueManager);

    m_imageAvailableSemaphore.initialize(m_device.get());
    m_finishedRenderSemaphore.initialize(m_device.get());
    
    m_renderPass.initialize(m_device.get(), m_surface.getFormat().format);
    
    Shader vs, fs;
    vs.initialize(RX_SHADER_PATH "test.vert", m_device.get());
    fs.initialize(RX_SHADER_PATH "test.frag", m_device.get());
    m_pipeline.initialize(m_device.get(), m_renderPass.get(), m_window, vs, fs);
    
    m_swapchain.initializeImages(m_device.get());
    m_swapchain.initializeImageViews(m_device.get(), m_surface);
    m_swapchain.initializeFramebuffers(m_device.get(), m_renderPass.get(), m_window);
    m_commandPool.initialize(m_device.get(), m_queueManager.getGraphicsIndex());
    m_commandBuffer.initialize(m_device.get(), m_commandPool.get());
  }

  bool Api::update()
  {
    return true;
  }

  bool Api::render()
  {
    uint32_t imageIndex = 0;

    // get image from swap chain
    VK_ASSERT(vkAcquireNextImageKHR(m_device.get(), m_swapchain.get(), VK_TIMEOUT, m_imageAvailableSemaphore.get(), VK_NULL_HANDLE, &imageIndex), "Failed to acquire next image from swap chain");

    m_commandPool.reset(m_device.get());

    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    // begin command buffer
    VK_ASSERT(vkBeginCommandBuffer(m_commandBuffer.get(), &beginInfo), "Failed to begin command buffer");
    {
      VkRenderPassBeginInfo renderPassBeginInfo = { };
      renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassBeginInfo.renderPass = m_renderPass.get();
      renderPassBeginInfo.framebuffer = m_swapchain.getFramebuffers()[imageIndex];

      int width, height;
      m_window->getSize(&width, &height);
      renderPassBeginInfo.renderArea.extent.width = static_cast<uint32_t>(width);
      renderPassBeginInfo.renderArea.extent.height = static_cast<uint32_t>(height);

      renderPassBeginInfo.clearValueCount = 1;

      VkClearValue color = { };
      color.color = { 0.2f, 0.2f, 0.2f, 1.0f };
      renderPassBeginInfo.pClearValues = &color;

      // begin render pass
      vkCmdBeginRenderPass(m_commandBuffer.get(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
      {
        {
        //VkViewport viewport = { };
        //viewport.x = 0.0f;
        //viewport.y = 0.0f;
        //viewport.width = static_cast<float>(width);
        //viewport.height = static_cast<float>(height);
        //viewport.minDepth = 0.0f;
        //viewport.maxDepth = 1.0f;
        //
        //VkRect2D scissor = { };
        //scissor.offset.x = 0;
        //scissor.offset.y = 0;
        //scissor.extent.width = static_cast<uint32_t>(width);
        //scissor.extent.height = static_cast<uint32_t>(height);
        //
        //vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
        //vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
        }
        
        // draw calls go here
        vkCmdBindPipeline(m_commandBuffer.get(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.get());
        vkCmdDraw(m_commandBuffer.get(), 3, 1, 0, 0);
      }
      // end render pass
      vkCmdEndRenderPass(m_commandBuffer.get());
    }
    // end command buffer
    VK_ASSERT(vkEndCommandBuffer(m_commandBuffer.get()), "Failed to end command buffer");

    VkPipelineStageFlags submitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{ };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    VkSemaphore m_imageAvailableSemaphores[] = { m_imageAvailableSemaphore.get() };
    submitInfo.pWaitSemaphores = m_imageAvailableSemaphores;
    submitInfo.pWaitDstStageMask = &submitStageMask;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffers[] = { m_commandBuffer.get() };
    submitInfo.pCommandBuffers = commandBuffers;
    submitInfo.signalSemaphoreCount = 1;
    VkSemaphore m_finishedRenderSemaphores[] = { m_finishedRenderSemaphore.get() };
    submitInfo.pSignalSemaphores = m_finishedRenderSemaphores;

    // submit queue
    m_queueManager.submit(submitInfo);
    //VK_ASSERT(vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit queue");

    VkPresentInfoKHR presentInfo{ };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = m_finishedRenderSemaphores;
    presentInfo.swapchainCount = 1;
    VkSwapchainKHR swapChains[] = { m_swapchain.get() };
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // present
    m_queueManager.present(presentInfo);
    //VK_ASSERT(vkQueuePresentKHR(m_queue, &presentInfo), "Failed to present");

    VK_ASSERT(vkDeviceWaitIdle(m_device.get()), "Device failed to wait idle");
    

    /*
    uint32_t imageIndex;
    vkAcquireNextImageKHR(m_device.get(), m_swapchain.get(), UINT64_MAX, m_imageAvailableSemaphore.get(), VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo{ };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore.get() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffers[] = { m_commandBuffer.get() };
    submitInfo.pCommandBuffers = commandBuffers;//&[imageIndex];

    VkSemaphore signalSemaphores[] = { m_finishedRenderSemaphore.get() };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);

    VkPresentInfoKHR presentInfo{ };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapchain.get() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(m_queue, &presentInfo);
    */
    /*
    The fix is probably to set up the present queue as well and differentitate between them
    */

    return true;
  }

  void Api::clean()
  {
    m_debugMessenger.destroy(m_instance.get());
  }
}