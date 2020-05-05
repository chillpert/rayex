#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphicsFamily;
  };

  class DeviceManager
  {
  public:
    DeviceManager();

    inline VkPhysicalDevice getPhysicalDevice() { return m_physicalDevice; }
    void findPhysicalDevice(VkInstance instance);

    VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void printPhysicalDeviceInfo();

  private:
    size_t evaluatePhysicalDevice(VkPhysicalDevice device);
    
    VkPhysicalDevice m_physicalDevice;
  };
}

#endif // PHYSICAL_DEVICE_MANAGER_HPP