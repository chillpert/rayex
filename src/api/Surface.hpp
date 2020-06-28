#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "WindowBase.hpp"

namespace RX
{
  class Surface
  {
  public:
    Surface();
    Surface(vk::Format format, vk::ColorSpaceKHR colorSpace, vk::PresentModeKHR presentMode, bool initialize = true);
    ~Surface();

    inline const vk::Format getFormat() const { return m_format; }
    inline const vk::ColorSpaceKHR getColorSpace() const { return m_colorSpace; }
    inline const vk::PresentModeKHR getPresentMode() const { return m_presentMode; }
    inline const vk::SurfaceCapabilitiesKHR getCapabilities() const { return m_capabilities; }

    // Creates the surface.
    void init();

    /*
      Checks if the preferred settings for format, color space and present mode are available. If not, the function will set them to some fallback values.
      Note: Must be called right after the enumeration of the physical device.
    */
    void checkSettingSupport();

  private:
    // Destroys the surface.
    void destroy();

    vk::SurfaceKHR m_surface;

    vk::Format m_format;
    vk::ColorSpaceKHR m_colorSpace;
    vk::PresentModeKHR m_presentMode;
    vk::SurfaceCapabilitiesKHR m_capabilities;
  };
}

#endif // SURFACE_HPP