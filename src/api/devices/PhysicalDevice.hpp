#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// A wrapper class for a Vulkan physical device.
  /// @ingroup API
  class PhysicalDevice
  {
  public:
    PhysicalDevice( ) = default;

    /// Retrieves the most suited GPU on the current machine. If a device is found its properties and features will be retrieved.
    /// @note SLI, Crossfire and similar are NOT supported.
    void init( );

  private:
    /// Scores a given physical device (GPU).
    /// @param physicalDevice The physical device to score.
    /// @return Returns a pair consisting out of the determined score and the name of the physical device.
    /// @note RTX hardware surpasses any other GPU.
    std::pair<unsigned int, std::string> evaluate( vk::PhysicalDevice physicalDevice ) const;

    vk::PhysicalDevice m_physicalDevice; ///< The Vulkan physical device.
  };

  /// This function is used to check if a device is suitable for the application's task.
  /// @param physicalDevice The physical device to check.
  /// @return Returns the GPU's supported ray tracing features.
  const vk::PhysicalDeviceRayTracingFeaturesKHR getRayTracingFeatures( vk::PhysicalDevice physicalDevice );
}

#endif // PHYSICAL_DEVICE_HPP