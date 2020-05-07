#include "api/vulkan/VulkanApi.hpp"
#include "pch/stdafx.hpp"

namespace RX
{
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;

  VulkanApi::VulkanApi() :
    m_instance { },
    m_surface { },
    m_device { VK_NULL_HANDLE },
    m_swapChain { m_surface.getSurface(), nullptr },
    m_pipeline{ VK_NULL_HANDLE, nullptr },
    m_commandBuffer { VK_NULL_HANDLE, VK_NULL_HANDLE, nullptr, nullptr } { }

  void VulkanApi::initialize(std::shared_ptr<Window> window)
  {
    Api::initialize(window);

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

  void VulkanApi::update()
  {
    
  }

  void VulkanApi::render()
  {
    uint32_t imageIndex;
    vkAcquireNextImageKHR(*m_device.getLogicalDevice(), *m_swapChain.getSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo { };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer.getCommandBuffers()[imageIndex];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    Assert::vulkan(vkQueueSubmit(*m_device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit draw command buffer");

    VkPresentInfoKHR presentInfo { };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { *m_swapChain.getSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(*m_device.getPresentQueue(), &presentInfo);
  }

  void VulkanApi::clean()
  {
    vkDestroySemaphore(*m_device.getLogicalDevice(), renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(*m_device.getLogicalDevice(), imageAvailableSemaphore, nullptr);

    m_commandBuffer.destroyCommandPool();

    m_swapChain.destroyFramebuffers();

    m_pipeline.destroyGraphicsPipeline();
    m_pipeline.destroyRenderPass();
    m_pipeline.destroyGraphicsPipelineLayout();

    m_swapChain.destroyImageView();
    m_swapChain.destroySwapChain();

    m_device.destroyLogicalDevice();

    m_surface.destroySurface();

    m_instance.destroyInstance();
  }

  void VulkanApi::createInstance()
  {
    m_instance.createInstance(m_window);
  }

  void VulkanApi::createDevices()
  {
    m_device = Device(m_instance.getInstance());

    m_device.pickPhysicalDevice();
    m_device.createLogicalDevice();
  }

  void VulkanApi::createSurface()
  {
    m_surface.createSurface(m_window, m_instance.getInstance());
  }

  void VulkanApi::createSwapChain()
  {
    m_swapChain = SwapChain(m_surface.getSurface(), &m_window->getProperties());

    m_swapChain.createSwapChain(m_device.getPhysicalDevice(), m_device.getLogicalDevice());
  }

  void VulkanApi::createImageViews()
  {
    m_swapChain.createImageViews();
  }

  void VulkanApi::createGraphicsPipeline()
  {
    m_pipeline = Pipeline(m_device.getLogicalDevice(), &m_swapChain);

    m_pipeline.createRenderPass();
    m_pipeline.createGraphicsPipeline();
  }

  void VulkanApi::createFramebuffers()
  {
    m_swapChain.createFramebuffers(m_pipeline.getRenderPass());
  }

  void VulkanApi::createCommandPool()
  {
    m_commandBuffer = CommandBuffer(m_device.getPhysicalDevice(), m_device.getLogicalDevice(), &m_swapChain, &m_pipeline);

    m_commandBuffer.createCommandPool();
  }

  void VulkanApi::createCommandBuffers()
  {
    m_commandBuffer.createCommandBuffers();
  }

  void VulkanApi::createSemaphores()
  {
    VkSemaphoreCreateInfo semaphoreInfo { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    Assert::vulkan(vkCreateSemaphore(*m_device.getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore), "Failed to create image available semaphore");
    Assert::vulkan(vkCreateSemaphore(*m_device.getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore), "Failed to create image available semaphore");

    
  }
}