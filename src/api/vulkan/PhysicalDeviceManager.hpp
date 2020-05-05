#ifndef PHYSICAL_DEVICE_MANAGER_HPP
#define PHYSICAL_DEVICE_MANAGER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class PhysicalDeviceManager
  {
  public:
    PhysicalDeviceManager();

    inline VkPhysicalDevice getDevice() { return m_device; }
    void findBestDevice(VkInstance instance);

    VkPhysicalDeviceProperties getDeviceProperties(VkPhysicalDevice device);
    VkPhysicalDeviceFeatures getDeviceFeatures(VkPhysicalDevice device);

    void printDeviceInfo();

  private:
    size_t evaluate(VkPhysicalDevice device);
    
    VkPhysicalDevice m_device;
  };
}

#endif // PHYSICAL_DEVICE_MANAGER_HPP