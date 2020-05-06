#include "api/vulkan/VulkanApi.hpp"
#include "pch/stdafx.hpp"

namespace RX
{
  VulkanApi::VulkanApi() :
    m_surface(VK_NULL_HANDLE),
    m_deviceManager(DeviceManager(m_instance.getInstance())),
    m_swapChain(SwapChain(&m_surface, &m_window->getProperties())) { }

  void VulkanApi::initialize(std::shared_ptr<Window> window)
  {
    Api::initialize(window);

    createInstance();
    createSurface();
    createDevices();
    createSwapChain();
  }

  void VulkanApi::update()
  {
    
  }

  void VulkanApi::render()
  {

  }

  void VulkanApi::clean()
  {
    m_swapChain.destroySwapChain();
    m_deviceManager.destroyLogicalDevice();
    vkDestroySurfaceKHR(*m_instance.getInstance(), m_surface, nullptr);
    m_instance.destroyInstance();
  }

  void VulkanApi::createInstance()
  {
    m_instance.createInstance(m_window);
  }

  void VulkanApi::createDevices()
  {
    m_deviceManager.pickPhysicalDevice();
    m_deviceManager.createLogicalDevice();
  }

  void VulkanApi::createSurface()
  {
    Assert::sdl(SDL_Vulkan_CreateSurface(m_window->getWindow(), *m_instance.getInstance(), &m_surface), "Failed to create surface");
  }

  void VulkanApi::createSwapChain()
  {
    m_swapChain.createSwapChain(m_deviceManager.getPhysicalDevice(), m_deviceManager.getLogicalDevice());
  }

  void VulkanApi::createImageViews()
  {

  }
}