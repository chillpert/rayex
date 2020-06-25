#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "InitializerInfos.hpp"

namespace RX
{
  class Surface
  {
  public:
    ~Surface();

    inline vk::SurfaceKHR get() { return m_surface; }
    inline vk::Format getFormat() const { return m_info.format; }
    inline vk::ColorSpaceKHR getColorSpace() const { return m_info.colorSpace; }
    inline vk::SurfaceCapabilitiesKHR getCapabilities() const { return m_capabilities; }
    inline vk::PresentModeKHR getPresentMode() const { return m_info.presentMode; }

    void initialize(SurfaceInfo& info);
    void destroy();

    // Checks if the preferred settings for format, color space and present mode are available.
    // If not, the function will set them to some fall back values.
    // Must be called right after the enumeration of the physical device.
    void checkSettingSupport(vk::PhysicalDevice physicalDevice);

  private:
    vk::SurfaceKHR m_surface;
    SurfaceInfo m_info;

    vk::SurfaceCapabilitiesKHR m_capabilities;
  };
}

#endif // SURFACE_HPP