#include "api/vulkan/VulkanApi.hpp"
#include "pch/stdafx.hpp"

namespace RX
{
  VulkanApi::VulkanApi() :
    m_instance { },
    m_surface { },
    m_deviceManager { VK_NULL_HANDLE },
    m_swapChain { m_surface.getSurface(), nullptr } { }

  void VulkanApi::initialize(std::shared_ptr<Window> window)
  {
    Api::initialize(window);

    createInstance();
    createSurface();
    createDevices();
    createSwapChain();
    createImageViews();
  }

  void VulkanApi::update()
  {
    
  }

  void VulkanApi::render()
  {

  }

  void VulkanApi::clean()
  {
    m_swapChain.destroyImageView();
    m_swapChain.destroySwapChain();
    m_deviceManager.destroyLogicalDevice();
    m_surface.destroySurface();
    m_instance.destroyInstance();
  }

  void VulkanApi::createInstance()
  {
    m_instance.createInstance(m_window);
  }

  void VulkanApi::createDevices()
  {
    m_deviceManager = DeviceManager(m_instance.getInstance());

    m_deviceManager.pickPhysicalDevice();
    m_deviceManager.createLogicalDevice();
  }

  void VulkanApi::createSurface()
  {
    m_surface.createSurface(m_window, m_instance.getInstance());
  }

  void VulkanApi::createSwapChain()
  {
    m_swapChain = SwapChain(m_surface.getSurface(), &m_window->getProperties());

    m_swapChain.createSwapChain(m_deviceManager.getPhysicalDevice(), m_deviceManager.getLogicalDevice());
  }

  void VulkanApi::createImageViews()
  {
    m_swapChain.createImageViews();
  }
}