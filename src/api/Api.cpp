#include "api/Api.hpp"

namespace RX
{
  Api::Api(std::shared_ptr<Window> window) :
    m_window(window),
    m_queue(VK_NULL_HANDLE) { }

  void Api::initialize()
  {
    uint32_t queueFamilyIndex = 0;

    // Required for extending the physical device from device extensions.
    instance.pushExtension("VK_KHR_get_physical_device_properties2");
    instance.create(m_window);
    instance.print();

    debugMessenger.create(instance.get());
    
    physicalDevice.create(instance.get());

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

    physicalDevice.checkExtensionSupport(requiredExtensions);

    device.create(instance.get(), physicalDevice.get(), &queueFamilyIndex);
    surface.create(instance.get(), m_window);
    swapchain.create(physicalDevice.get(), device.get(), surface, m_window, &queueFamilyIndex);

    imageAvailableSemaphore.create(device.get());
    finishedRenderSemaphore.create(device.get());
    
    vkGetDeviceQueue(device.get(), queueFamilyIndex, 0, &m_queue); // TODO: create queue abstraction
    
    renderPass.create(device.get(), surface.getFormat().format);
    
    vs.create(RX_SHADER_PATH "test.vert", device.get());
    fs.create(RX_SHADER_PATH "test.frag", device.get());
    pipeline.create(device.get(), renderPass.get(), m_window, vs, fs);
    
    swapchain.createImages(device.get());
    swapchain.createImageViews(device.get(), surface);
    swapchain.createFramebuffers(device.get(), renderPass.get(), m_window);
    commandPool.create(device.get(), &queueFamilyIndex);
    commandBuffer.create(device.get(), commandPool.get());
  }

  bool Api::update()
  {
    return true;
  }

  bool Api::render()
  {
    uint32_t imageIndex = 0;

    // get image from swap chain
    VK_ASSERT(vkAcquireNextImageKHR(device.get(), swapchain.get(), VK_TIMEOUT, imageAvailableSemaphore.get(), VK_NULL_HANDLE, &imageIndex), "Failed to acquire next image from swap chain");

    commandPool.reset(device.get());

    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    // begin command buffer
    VK_ASSERT(vkBeginCommandBuffer(commandBuffer.get(), &beginInfo), "Failed to begin command buffer");
    {
      VkRenderPassBeginInfo renderPassBeginInfo = { };
      renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassBeginInfo.renderPass = renderPass.get();
      renderPassBeginInfo.framebuffer = swapchain.getFramebuffers()[imageIndex];

      int width, height;
      m_window->getWindowSize(&width, &height);
      renderPassBeginInfo.renderArea.extent.width = static_cast<uint32_t>(width);
      renderPassBeginInfo.renderArea.extent.height = static_cast<uint32_t>(height);

      renderPassBeginInfo.clearValueCount = 1;

      VkClearValue color = { };
      color.color = { 0.2f, 0.2f, 0.2f, 1.0f };
      renderPassBeginInfo.pClearValues = &color;

      // begin render pass
      vkCmdBeginRenderPass(commandBuffer.get(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
      {
        /*
        VkViewport viewport = { };
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(width);
        viewport.height = static_cast<float>(height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = { };
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent.width = static_cast<uint32_t>(width);
        scissor.extent.height = static_cast<uint32_t>(height);

        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
        */
        
        // draw calls go here
        vkCmdBindPipeline(commandBuffer.get(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
        vkCmdDraw(commandBuffer.get(), 3, 1, 0, 0);
      }
      // end render pass
      vkCmdEndRenderPass(commandBuffer.get());
    }
    // end command buffer
    VK_ASSERT(vkEndCommandBuffer(commandBuffer.get()), "Failed to end command buffer");

    VkPipelineStageFlags submitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{ };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    VkSemaphore imageAvailableSemaphores[] = { imageAvailableSemaphore.get() };
    submitInfo.pWaitSemaphores = imageAvailableSemaphores;
    submitInfo.pWaitDstStageMask = &submitStageMask;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffers[] = { commandBuffer.get() };
    submitInfo.pCommandBuffers = commandBuffers;
    submitInfo.signalSemaphoreCount = 1;
    VkSemaphore finishedRenderSemaphores[] = { finishedRenderSemaphore.get() };
    submitInfo.pSignalSemaphores = finishedRenderSemaphores;

    // submit queue
    VK_ASSERT(vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit queue");

    VkPresentInfoKHR presentInfo{ };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = finishedRenderSemaphores;
    presentInfo.swapchainCount = 1;
    VkSwapchainKHR swapChains[] = { swapchain.get() };
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // present
    VK_ASSERT(vkQueuePresentKHR(m_queue, &presentInfo), "Failed to present");

    VK_ASSERT(vkDeviceWaitIdle(device.get()), "Device failed to wait idle");

    return true;
  }

  void Api::clean()
  {
    debugMessenger.destroy(instance.get());
  }
}