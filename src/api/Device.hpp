#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Device
  {
  public:
    ~Device();

    inline VkDevice get() { return device; }

    void create(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t* queueFamilyIndex);
    // No need to call destroy at application shutdown.
    void destroy();

  private:
    VkDevice device;
  };
}

#endif // DEVICE_HPP