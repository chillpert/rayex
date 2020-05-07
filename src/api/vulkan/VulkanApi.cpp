#include "api/vulkan/VulkanApi.hpp"
#include "pch/stdafx.hpp"

namespace RX
{
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
  }

  void VulkanApi::update()
  {
    
  }

  void VulkanApi::render()
  {

  }

  void VulkanApi::clean()
  {
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
}