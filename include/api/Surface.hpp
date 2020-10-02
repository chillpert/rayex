#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "base/Window.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A wrapper class for a Vulkan surface.
  /// @ingroup API
  class Surface
  {
  public:
    /// Calls destroy().
    ~Surface( );

    /// @return Returns the surface format.
    inline const vk::Format getFormat( ) const { return format; }

    /// @return Returns the surface's color space.
    inline const vk::ColorSpaceKHR getColorSpace( ) const { return colorSpace; }

    /// @return Returns the surface's present mode.
    inline const vk::PresentModeKHR getPresentMode( ) const { return presentMode; }

    /// @return Returns the surface's capabilities.
    inline const vk::SurfaceCapabilitiesKHR getCapabilities( ) const { return capabilities; }

    /// Initializes the Vulkan surface object.
    /// @note If any of the specified format, color space and present mode are not available the function will fall back to settings that are guaranteed to be supported.
    void init( );

    /// Checks if the preferred settings for format, color space and present mode are available. If not, the function will set them to some fallback values.
    /// @warning Must be called right after the enumeration of the physical device.
    void assessSettings( );

  private:
    /// Destroys the surface.
    void destroy( );

    vk::SurfaceKHR surface; ///< The Vulkan surface.

    vk::Format format                       = vk::Format::eB8G8R8A8Unorm;        ///< The desired surface format.
    vk::ColorSpaceKHR colorSpace            = vk::ColorSpaceKHR::eSrgbNonlinear; ///< The desired color space.
    vk::PresentModeKHR presentMode          = vk::PresentModeKHR::eMailbox;      ///< The desired present mode.
    vk::SurfaceCapabilitiesKHR capabilities = 0;                                 ///< The surface's capabilities.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SURFACE_HPP