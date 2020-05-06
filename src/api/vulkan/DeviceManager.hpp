#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
  };

  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  class DeviceManager
  {
  public:
    DeviceManager(VkInstance* instance, VkSurfaceKHR* surface);

    void pickPhysicalDevice();
    void createLogicalDevice();

    void destroyLogicalDevice();

    inline VkPhysicalDevice getPhysicalDevice() { return m_physicalDevice; }
    inline VkDevice getLogicalDevice() { return m_logicalDevice; }

  private:
    // physical
    size_t evaluatePhysicalDevice(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void printPhysicalDeviceInfo();

    // logical
    
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_logicalDevice;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    // pointer to VulkanApi class
    VkInstance* m_instance;
    VkSurfaceKHR* m_surface;
  };
}

#endif // PHYSICAL_DEVICE_MANAGER_HPP