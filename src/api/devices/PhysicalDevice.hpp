#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct PhysicalDeviceInfo
  {
    vk::Instance instance;
    vk::SurfaceKHR surface;
  };

  class PhysicalDevice
  {
  public:
    PhysicalDevice() = default;
    PhysicalDevice(PhysicalDeviceInfo& info);
    PhysicalDevice(PhysicalDeviceInfo&& info);

    // Technically not created, but enumerated. However, this way is better for the naming scheme.
    void init(PhysicalDeviceInfo& info);
    void init(PhysicalDeviceInfo&& info);

    inline vk::PhysicalDevice get() { return m_physicalDevice; }

    inline vk::PhysicalDeviceProperties getProperties() const { return m_properties; }
    inline vk::PhysicalDeviceProperties2 getProperties2() const { return m_properties2; }
    inline vk::PhysicalDeviceFeatures getFeatures() const { return m_features; }
    inline vk::PhysicalDeviceFeatures2 getFeatures2() const { return m_features2; }

  private:
    std::pair<unsigned int, std::string> evaluate(vk::PhysicalDevice physicalDevice) const;

    vk::PhysicalDevice m_physicalDevice;
    PhysicalDeviceInfo m_info;

    vk::PhysicalDeviceProperties m_properties;
    vk::PhysicalDeviceProperties2 m_properties2;
    vk::PhysicalDeviceFeatures m_features;
    vk::PhysicalDeviceFeatures2 m_features2;
  };
}

#endif // PHYSICAL_DEVICE_HPP