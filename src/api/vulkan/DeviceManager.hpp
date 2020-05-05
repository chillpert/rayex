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

    void createDevices(VkInstance instance);
    void destroyDevices();

    inline VkPhysicalDevice getPhysicalDevice() { return m_physicalDevice; }
    inline VkDevice getLogicalDevice() { return m_logicalDevice; }

  private:
    // physical
    void findPhysicalDevice(VkInstance instance);

    size_t evaluatePhysicalDevice(VkPhysicalDevice device);
    VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void printPhysicalDeviceInfo();

    // logical
    void createLogicalDevice();
    
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_logicalDevice;
  };
}

#endif // PHYSICAL_DEVICE_MANAGER_HPP