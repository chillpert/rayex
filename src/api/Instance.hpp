#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "WindowBase.hpp"

namespace rx
{
  /// A wrapper class for the Vulkan instance.
  /// @ingroup API
  class Instance
  {
  public:
    Instance( ) = default;
    /// @param layers All validation layers that should be activated.
    /// @param extensiosn All instance extensions that should be activated.
    /// @param initialize If true, the instance object will be initialized right away without an additional call to init().
    Instance( const std::vector<const char*>& layers, std::vector<const char*>& extensions, bool initialize = true );

    /// Initializes the instance object and checks support for any given layer and extension.
    /// @param layers All validation layers that should be activated.
    /// @param extensions All instance extensions that should be activated.
    /// @note The given vector with the extensions will be appended by ones that are required by the window.
    void init( const std::vector<const char*>& layers, std::vector<const char*>& extensions );

  private:
    /// Checks if all layers provided are supported.
    /// 
    /// If any layer is not supported the application will shut down immediately.
    /// @param layers All validation layers that should be activated.
    void checkLayersSupport( const std::vector<const char*>& layers );

    /// Checks if all instance extensions provided are supported.
    /// 
    /// If any instance extension is not supported the application will shut dow immediately.
    /// @param extensions All instance extensions that should be activated.
    void checkExtensionsSupport( const std::vector<const char*>& extensions );

    /// Returns the version of the Vulkan SDK installed on the machine. 
    /// @note It can not retrieve patch version, only major and minor version.
    uint32_t getApiVersion( );

    vk::UniqueInstance m_instance; ///< The Vulkan instance with a unique handle.
  };
}

#endif // INSTANCE_HPP