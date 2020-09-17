#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "api/Instance.hpp"

namespace RENDERER_NAMESPACE
{
  /// A wrapper class for a Vulkan logical device.
  /// @ingroup API
  class Device
  {
  public:
    Device( ) = default;

    /// @param extensions All device extensions that should be activated.
    /// @param initialize If true, the logical device object will be initialized right away without an additional call to init().
    Device( std::vector<const char*>& extensions, bool initialize = true );
    
    /// Creates the logical device and enables all device extensions provided if they are supported.
    /// @param queueFamilies All queue families whose queues should be created.
    /// @param extensions All device extensions that should be enabled.
    void init( std::vector<const char*>& extensions );

  private:
    /// Checks if all device extensions provided are supported.
    /// 
    /// If any device extension is not supported the application will shut down immediately.
    /// @param extensions All device extensions that should be activated.
    void checkExtensionSupport( const std::vector<const char*>& extensions ) const;

    vk::UniqueDevice device; ///< The Vulkan logical device with a unique handle.
  };
}

#endif // DEVICE_HPP