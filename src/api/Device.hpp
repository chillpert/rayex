#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "pch/stdafx.hpp"
#include "Instance.hpp"

namespace RX
{
  class Device
  {
  public:
    inline VkDevice get() { return device; }

    void create(VkPhysicalDevice physicalDevice);
    void destroy();

    // To verify the support of any device extension, use PhysicalDevice::checkExtensionSupport.
    void pushExtension(const char* name);

  private:
    VkDevice device;

    std::vector<const char*> extensions;
    bool created = false;
  };
}

#endif // DEVICE_HPP