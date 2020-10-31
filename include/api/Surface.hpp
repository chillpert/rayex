#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "base/Window.hpp"

namespace RAYEX_NAMESPACE
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
    auto operator=( const Surface&& ) -> Surface& = delete;

    /// @return Returns the surface format.
    auto getFormat( ) const -> vk::Format { return _format; }

    /// @return Returns the surface's color space.
    auto getColorSpace( ) const -> vk::ColorSpaceKHR { return _colorSpace; }

    /// @return Returns the surface's present mode.
    auto getPresentMode( ) const -> vk::PresentModeKHR { return _presentMode; }

    /// @return Returns the surface's capabilities.
    auto getCapabilities( ) const -> vk::SurfaceCapabilitiesKHR { return _capabilities; }

    /// @return Returns the surface's extent.
    auto getExtent( ) const -> vk::Extent2D { return _extent; }

    /// Initializes the Vulkan surface object.
    /// @param window A window to create a surface for.
    /// @note If any of the specified format, color space and present mode are not available the function will fall back to settings that are guaranteed to be supported.
    void init( std::shared_ptr<Window> window );

    /// Checks if the preferred settings for format, color space and present mode are available. If not, the function will set them to some fallback values.
    /// @warning Must be called right after the enumeration of the physical device.
    void assessSettings( );

  private:
    /// Destroys the surface.
    void destroy( );

    vk::SurfaceKHR _surface                  = nullptr;                           ///< The Vulkan surface.
    vk::Format _format                       = vk::Format::eB8G8R8A8Unorm;        ///< The desired surface format.
    vk::ColorSpaceKHR _colorSpace            = vk::ColorSpaceKHR::eSrgbNonlinear; ///< The desired color space.
    vk::PresentModeKHR _presentMode          = vk::PresentModeKHR::eFifoRelaxed;  ///< The desired present mode.
    vk::SurfaceCapabilitiesKHR _capabilities = 0;                                 ///< The surface's capabilities.
    vk::Extent2D _extent                     = { };                               ///< The surface's extent.
  };
} // namespace RAYEX_NAMESPACE

#endif // SURFACE_HPP
