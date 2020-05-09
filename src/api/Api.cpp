#include "api/Api.hpp"
#include "pch/stdafx.hpp"

namespace RX
{
  const int MAX_FRAMES_IN_FLIGHT = 2;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;

  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;

  size_t currentFrame = 0;

  Api::Api(std::shared_ptr<Window> window) :
    m_instance{ },
    m_surface{ },
    m_device{ VK_NULL_HANDLE },
    m_swapChain{ m_surface.getSurface(), nullptr },
    m_pipeline{ VK_NULL_HANDLE, nullptr },
    m_commandBuffer{ VK_NULL_HANDLE, VK_NULL_HANDLE, nullptr, nullptr },
    m_window(window) { }

  void Api::initialize()
  {
    createInstance();
    createSurface();
    createDevices();
    createSwapChain();
    createImageViews();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffers();
    createSemaphores();
  }

  void Api::update()
  {
    
  }

  void Api::render()
  {
    vkWaitForFences(*m_device.getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(*m_device.getLogicalDevice(), *m_swapChain.getSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
      recreateSwapChain();
      return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
      Error::runtime("Failed to acquire swap chain image", Error::SWAPCHAIN);
    }

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
      vkWaitForFences(*m_device.getLogicalDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo { };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer.getCommandBuffers()[imageIndex];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(*m_device.getLogicalDevice(), 1, &inFlightFences[currentFrame]);

    Assert::vulkan(vkQueueSubmit(*m_device.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]), "Failed to submit draw command buffer");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { *m_swapChain.getSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(*m_device.getPresentQueue(), &presentInfo);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  void Api::clean()
  {
    vkDeviceWaitIdle(*m_device.getLogicalDevice());

    cleanSwapChain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
      vkDestroySemaphore(*m_device.getLogicalDevice(), renderFinishedSemaphores[i], nullptr);
      vkDestroySemaphore(*m_device.getLogicalDevice(), imageAvailableSemaphores[i], nullptr);
      vkDestroyFence(*m_device.getLogicalDevice(), inFlightFences[i], nullptr);
    }

    m_commandBuffer.destroyCommandPool();

    m_device.destroyLogicalDevice();

    m_surface.destroySurface();

    m_instance.destroyInstance();    
  }

  void Api::createInstance()
  {
    m_instance.createInstance(m_window);
  }

  void Api::createDevices()
  {
    m_device = Device(m_instance.getInstance());

    m_device.pickPhysicalDevice();
    m_device.createLogicalDevice();
  }

  void Api::createSurface()
  {
    m_surface.createSurface(m_window, m_instance.getInstance());
  }

  void Api::createSwapChain()
  {
    m_swapChain = SwapChain(m_surface.getSurface(), m_window);

    m_swapChain.createSwapChain(m_device.getPhysicalDevice(), m_device.getLogicalDevice());
  }

  void Api::createImageViews()
  {
    m_swapChain.createImageViews();
  }

  void Api::createGraphicsPipeline()
  {
    m_pipeline = Pipeline(m_device.getLogicalDevice(), &m_swapChain);

    m_pipeline.createRenderPass();
    m_pipeline.createGraphicsPipeline();
  }

  void Api::createFramebuffers()
  {
    m_swapChain.createFramebuffers(m_pipeline.getRenderPass());
  }

  void Api::createCommandPool()
  {
    m_commandBuffer = CommandBuffer(m_device.getPhysicalDevice(), m_device.getLogicalDevice(), &m_swapChain, &m_pipeline);

    m_commandBuffer.createCommandPool();
  }

  void Api::createCommandBuffers()
  {
    m_commandBuffer.createCommandBuffers();
  }

  void Api::createSemaphores()
  {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(m_swapChain.getImages().size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo { };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
      Assert::vulkan(vkCreateSemaphore(*m_device.getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]), "Failed to create image available semaphore");
      Assert::vulkan(vkCreateSemaphore(*m_device.getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]), "Failed to create finished rendering semaphore");
      Assert::vulkan(vkCreateFence(*m_device.getLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]), "Failed to create fence");
    }
  }

  void Api::cleanSwapChain()
  {
    m_swapChain.destroyFramebuffers();

    m_commandBuffer.freeCommandBuffers();

    m_pipeline.destroyGraphicsPipeline();
    m_pipeline.destroyGraphicsPipelineLayout();
    m_pipeline.destroyRenderPass();

    m_swapChain.destroyImageView();
    m_swapChain.destroySwapChain();
  }
  
  void Api::recreateSwapChain()
  {
    vkDeviceWaitIdle(*m_device.getLogicalDevice());

    // Handle the window getting minimized.
    int w = m_window->getProperties().getWidth();
    int h = m_window->getProperties().getHeight();

    while (w <= 0 || h <= 0)
    {
      SDL_GetWindowSize(m_window->getWindow(), &w, &h);
    }

    cleanSwapChain();

    createSwapChain();
    createImageViews();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandBuffers();
  }
}