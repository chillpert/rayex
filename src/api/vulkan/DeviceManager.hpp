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

  class DeviceManager
  {
  public:
    DeviceManager();

    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    void createLogicalDevice(VkInstance instance, VkSurfaceKHR surface);

    void destroyLogicalDevice();

    inline VkPhysicalDevice getPhysicalDevice() { return m_physicalDevice; }
    inline VkDevice getLogicalDevice() { return m_logicalDevice; }

  private:
    // physical
    size_t evaluatePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    void printPhysicalDeviceInfo();

    // logical
    
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_logicalDevice;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
  };
}

#endif // PHYSICAL_DEVICE_MANAGER_HPP