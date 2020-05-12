#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Device
  {
  public:
    inline VkDevice get() { return device; }

    void create(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t* queueFamilyIndex);
    void destroy();

  private:
    VkDevice device;
  };
}

#endif // DEVICE_HPP