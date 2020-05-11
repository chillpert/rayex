#include "api/VkApi.hpp"
#include "api/VkUtils.hpp"

namespace RX
{
  VkApi::VkApi(std::shared_ptr<Window> window) :
    m_window(window),
    m_swapChainFormat(VK_FORMAT_B8G8R8A8_UNORM),
    m_renderPass(VK_NULL_HANDLE),
    m_pipeline(VK_NULL_HANDLE),
    m_queue(VK_NULL_HANDLE) { }

  void VkApi::initialize()
  {
    uint32_t queueFamilyIndex = 0;

    instance.create(m_window);
    m_messenger.create(instance.get());
    physicalDevice.create(instance.get());
    device.create(instance.get(), physicalDevice.get(), &queueFamilyIndex);
    surface.create(instance.get(), m_window);
    swapchain.create(physicalDevice.get(), device.get(), surface, m_window, &queueFamilyIndex);

    imageAvailableSemaphore.create(device.get());
    finishedRenderSemaphore.create(device.get());
    
    vkGetDeviceQueue(device.get(), queueFamilyIndex, 0, &m_queue);
    
    m_renderPass = createRenderPass(device.get(), m_swapChainFormat);
    
    m_vertexShader = std::make_shared<VkShader>(RX_SHADER_PATH, "test.vert", device.get());
    m_fragmentShader = std::make_shared<VkShader>(RX_SHADER_PATH, "test.frag", device.get());
    m_pipeline = createPipeline(device.get(), m_renderPass, m_window, m_vertexShader, m_fragmentShader);
    
    uint32_t swapChainImageCount;
    Assert::vulkan(
      vkGetSwapchainImagesKHR(device.get(), swapchain.get(), &swapChainImageCount, nullptr),
      "Failed to get swap chain images"
    );

    // TODO: move all the stuff below in another function
    m_swapChainImages.resize(swapChainImageCount);
    Assert::vulkan(
      vkGetSwapchainImagesKHR(device.get(), swapchain.get(), &swapChainImageCount, m_swapChainImages.data()),
      "Failed to get swap chain images"
    );

    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImageCount; i++)
    {
      m_swapChainImageViews[i] = createImageView(device.get(), m_swapChainImages[i], m_swapChainFormat);
    }

    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (uint32_t i = 0; i < swapChainImageCount; i++)
    {
      m_swapChainFramebuffers[i] = createFramebuffer(device.get(), m_renderPass, m_swapChainImageViews[i], m_window);
    }

    commandPool.create(device.get(), &queueFamilyIndex);
    commandBuffer.create(device.get(), commandPool.get());
  }

  bool VkApi::update()
  {
    return true;
  }

  bool VkApi::render()
  {
    uint32_t imageIndex = 0;

    VK_ASSERT(vkAcquireNextImageKHR(device.get(), swapchain.get(), VK_TIMEOUT, imageAvailableSemaphore.get(), VK_NULL_HANDLE, &imageIndex), "Failed to acquire next image from swap chain");

    commandPool.reset(device.get());

    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_ASSERT(vkBeginCommandBuffer(commandBuffer.get(), &beginInfo), "Failed to begin command buffer");

    VkRenderPassBeginInfo renderPassBeginInfo = { };
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.framebuffer = m_swapChainFramebuffers[imageIndex];

    int width, height;
    m_window->getWindowSize(&width, &height);
    renderPassBeginInfo.renderArea.extent.width = static_cast<uint32_t>(width);
    renderPassBeginInfo.renderArea.extent.height = static_cast<uint32_t>(height);

    renderPassBeginInfo.clearValueCount = 1;

    VkClearValue color = { };
    color.color = { 0.2f, 0.2f, 0.2f, 1.0f };
    renderPassBeginInfo.pClearValues = &color;

    vkCmdBeginRenderPass(commandBuffer.get(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
     
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
    vkCmdBindPipeline(commandBuffer.get(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdDraw(commandBuffer.get(), 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer.get());
    
    Assert::vulkan(
      vkEndCommandBuffer(commandBuffer.get()),
      "Failed to end command buffer"
    );

    VkPipelineStageFlags submitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = { };
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

    Assert::vulkan(
      vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE),
      "Failed to submit queue"
    );

    VkPresentInfoKHR presentInfo = { };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = finishedRenderSemaphores;
    presentInfo.swapchainCount = 1;
    VkSwapchainKHR swapChains[] = { swapchain.get() };
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    Assert::vulkan(
      vkQueuePresentKHR(m_queue, &presentInfo),
      "Failed to present"
    );

    Assert::vulkan(
      vkDeviceWaitIdle(device.get()),
      "Device failed to wait idle"
    );

    return true;
  }

  void VkApi::clean()
  {
    m_messenger.destroy(instance.get());
  }
}