#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  class PhysicalDevice
  {
  public:
    PhysicalDevice( ) = default;

    // Retrieves the most suited GPU on the current machine. If a device is found its properties and features will be retrieved.
    // Note: SLI, Crossfire and similar is NOT supported.
    void init( );

    // Returns the device's properties.
    // Note: Must be called after PhysicalDevice::init().
    const vk::PhysicalDeviceProperties getProperties( ) const;

    // Returns the device's second properties.
    // Note: Must be called after PhysicalDevice::init().
    const vk::PhysicalDeviceProperties2 getProperties2( ) const;

    // Returns the device's features.
    // Note: Must be called after PhysicalDevice::init().
    const vk::PhysicalDeviceFeatures getFeatures( ) const;

    // Returns the device's second features.
    // Note: Must be called after PhysicalDevice::init().
    const vk::PhysicalDeviceFeatures2 getFeatures2( ) const;

  private:
    // Scores a given physical device (GPU).
    // @param physicalDevice - The physical device to score.
    // @return - Returns a pair consisting out of the determined score and the name of the physical device.
    // Note: RTX hardware surpasses any other GPU.
    std::pair<unsigned int, std::string> evaluate( vk::PhysicalDevice physicalDevice ) const;

    vk::PhysicalDevice m_physicalDevice;

    vk::PhysicalDeviceProperties m_properties;
    vk::PhysicalDeviceProperties2 m_properties2;
    vk::PhysicalDeviceFeatures m_features;
    vk::PhysicalDeviceFeatures2 m_features2;
  };

  // This function is used to check if a device is suitable for the application's task.
  // @param physicalDevice - The physical device to check.
  // @return - Returns the GPU's supported ray tracing features.
  const vk::PhysicalDeviceRayTracingFeaturesKHR getRayTracingFeatures( vk::PhysicalDevice physicalDevice );
}

#endif // PHYSICAL_DEVICE_HPP