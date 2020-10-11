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
    Surface( ) = default;
    /// Calls destroy().
    ~Surface( );

    Surface( const Surface& )  = delete;
    Surface( const Surface&& ) = delete;

    auto operator=( const Surface& ) -> Surface& = delete;
    auto operator=( const Surface && ) -> Surface& = delete;

    /// @return Returns the surface format.
    [[nodiscard]] inline auto getFormat( ) const -> vk::Format { return format; }

    /// @return Returns the surface's color space.
    [[nodiscard]] inline auto getColorSpace( ) const -> vk::ColorSpaceKHR { return colorSpace; }

    /// @return Returns the surface's present mode.
    [[nodiscard]] inline auto getPresentMode( ) const -> vk::PresentModeKHR { return presentMode; }

    /// @return Returns the surface's capabilities.
    [[nodiscard]] inline auto getCapabilities( ) const -> vk::SurfaceCapabilitiesKHR { return capabilities; }

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
    vk::PresentModeKHR presentMode          = vk::PresentModeKHR::eImmediate;    ///< The desired present mode.
    vk::SurfaceCapabilitiesKHR capabilities = 0;                                 ///< The surface's capabilities.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SURFACE_HPP
