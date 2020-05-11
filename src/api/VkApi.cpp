#include "api/VkApi.hpp"
#include "api/VkUtils.hpp"

namespace RX
{
  VkApi::VkApi(std::shared_ptr<Window> window) :
    m_window(window),
    m_instance(VK_NULL_HANDLE),
    m_physicalDevice(VK_NULL_HANDLE),
    m_device(VK_NULL_HANDLE),
    m_surface(VK_NULL_HANDLE),
    m_swapChain(VK_NULL_HANDLE),
    m_swapChainFormat(VK_FORMAT_B8G8R8A8_UNORM),
    m_renderPass(VK_NULL_HANDLE),
    m_pipeline(VK_NULL_HANDLE),
    m_imageAvailableSemaphore(VK_NULL_HANDLE),
    m_finishedRenderSemaphore(VK_NULL_HANDLE),
    m_queue(VK_NULL_HANDLE),
    m_commandPool(VK_NULL_HANDLE),
    m_commandBuffer(VK_NULL_HANDLE) { }

  void VkApi::initialize()
  {
    uint32_t queueFamilyIndex = 0;

    m_instance = createInstance(m_window);
    m_messenger.create(m_instance);
    m_physicalDevice = pickPhysicalDevice(m_instance);
    m_device = createDevice(m_instance, m_physicalDevice, &queueFamilyIndex);
    m_surface = m_window->createSurface(m_instance);
    m_swapChain = createSwapChain(m_physicalDevice, m_device, m_surface, m_window, &queueFamilyIndex, &m_swapChainFormat);

    m_imageAvailableSemaphore = createSemaphore(m_device);
    m_finishedRenderSemaphore = createSemaphore(m_device);
    
    vkGetDeviceQueue(m_device, queueFamilyIndex, 0, &m_queue);
    
    m_renderPass = createRenderPass(m_device, m_swapChainFormat);
    
    m_vertexShader = std::make_shared<VkShader>(RX_SHADER_PATH, "test.vert", &m_device);
    m_fragmentShader = std::make_shared<VkShader>(RX_SHADER_PATH, "test.frag", &m_device);
    m_pipeline = createPipeline(m_device, m_renderPass, m_window, m_vertexShader, m_fragmentShader);
    
    uint32_t swapChainImageCount;
    Assert::vulkan(
      vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImageCount, nullptr),
      "Failed to get swap chain images"
    );

    // TODO: move all the stuff below in another function
    m_swapChainImages.resize(swapChainImageCount);
    Assert::vulkan(
      vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImageCount, m_swapChainImages.data()),
      "Failed to get swap chain images"
    );

    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImageCount; i++)
    {
      m_swapChainImageViews[i] = createImageView(m_device, m_swapChainImages[i], m_swapChainFormat);
    }

    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (uint32_t i = 0; i < swapChainImageCount; i++)
    {
      m_swapChainFramebuffers[i] = createFramebuffer(m_device, m_renderPass, m_swapChainImageViews[i], m_window);
    }

    m_commandPool = createCommandPool(m_device, &queueFamilyIndex);

    VkCommandBufferAllocateInfo allocateInfo = { };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = m_commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    Assert::vulkan(
      vkAllocateCommandBuffers(m_device, &allocateInfo, &m_commandBuffer),
      "Failed to allocate command buffers"
    );
  }

  bool VkApi::update()
  {
    return true;
  }

  bool VkApi::render()
  {
    uint32_t imageIndex = 0;

    Assert::vulkan(
      vkAcquireNextImageKHR(m_device, m_swapChain, VK_TIMEOUT, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex),
      "Failed to acquire next image from swap chain"
    );

    Assert::vulkan(
      vkResetCommandPool(m_device, m_commandPool, 0),
      "Failed to reset command pool"
    );

    VkCommandBufferBeginInfo beginInfo = { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    Assert::vulkan(
      vkBeginCommandBuffer(m_commandBuffer, &beginInfo),
      "Failed to begin command buffer"
    );

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

    vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
     
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
    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(m_commandBuffer);
    
    Assert::vulkan(
      vkEndCommandBuffer(m_commandBuffer),
      "Failed to end command buffer"
    );

    VkPipelineStageFlags submitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = { };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_imageAvailableSemaphore;
    submitInfo.pWaitDstStageMask = &submitStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_finishedRenderSemaphore;

    Assert::vulkan(
      vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE),
      "Failed to submit queue"
    );

    VkPresentInfoKHR presentInfo = { };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_finishedRenderSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain;
    presentInfo.pImageIndices = &imageIndex;

    Assert::vulkan(
      vkQueuePresentKHR(m_queue, &presentInfo),
      "Failed to present"
    );

    Assert::vulkan(
      vkDeviceWaitIdle(m_device),
      "Device failed to wait idle"
    );

    return true;
  }

  void VkApi::clean()
  {
    m_messenger.destroy(m_instance);
  }
}