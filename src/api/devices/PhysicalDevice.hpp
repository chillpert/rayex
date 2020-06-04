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

    inline VkPhysicalDeviceProperties getProperties() const { return m_properties; }
    inline VkPhysicalDeviceProperties2 getProperties2() const { return m_properties2; }
    inline VkPhysicalDeviceFeatures getFeatures() const { return m_features; }
    inline VkPhysicalDeviceFeatures2 getFeatures2() const { return m_features2; }

  private:
    std::pair<unsigned int, std::string> evaluate(VkPhysicalDevice device) const;

    VkPhysicalDevice m_physicalDevice;
    PhysicalDeviceInfo m_info;

    VkPhysicalDeviceProperties m_properties;
    VkPhysicalDeviceProperties2 m_properties2;
    VkPhysicalDeviceFeatures m_features;
    VkPhysicalDeviceFeatures2 m_features2;
  };
}

#endif // PHYSICAL_DEVICE_HPP