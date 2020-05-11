#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class PhysicalDevice
  {
  public:
    inline VkPhysicalDevice get() { return physicalDevice; }

    // Technically not created, but enumerated. However, this way is better for the naming scheme.
    void create(VkInstance instance);
  
  private:
    VkPhysicalDevice physicalDevice;
  };
}

#endif // PHYSICAL_DEVICE_HPP