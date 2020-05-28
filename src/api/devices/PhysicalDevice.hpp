#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct PhysicalDeviceInfo
  {
    VkInstance instance;
    VkSurfaceKHR surface;
  };

  class PhysicalDevice
  {
  public:
    // Technically not created, but enumerated. However, this way is better for the naming scheme.
    void initialize(PhysicalDeviceInfo& info);

    inline VkPhysicalDevice get() { return m_physicalDevice; }
    inline PhysicalDeviceInfo& getInfo() { return m_info; }

  private:
    unsigned int evaluate(VkPhysicalDevice device) const;

    VkPhysicalDevice m_physicalDevice;
    PhysicalDeviceInfo m_info;
  };
}

#endif // PHYSICAL_DEVICE_HPP