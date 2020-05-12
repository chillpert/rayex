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
  
    // Checks if all given physical device extensions are available.
    void checkExtensionSupport(const std::vector<const char*>& extensions) const;

  private:
    VkPhysicalDevice physicalDevice;
  };
}

#endif // PHYSICAL_DEVICE_HPP