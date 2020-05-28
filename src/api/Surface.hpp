#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "window/Window.hpp"

namespace RX
{
  struct SurfaceInfo
  {
    std::shared_ptr<Window> window;
    VkInstance instance;
    VkFormat format = VK_FORMAT_B8G8R8A8_UNORM;
    VkColorSpaceKHR colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    VkSurfaceCapabilitiesKHR capabilities; // Ignore, will be initilized automatically.
  };

  class Surface
  {
  public:
    ~Surface();

    inline VkSurfaceKHR get() { return m_surface; }
    inline SurfaceInfo& getInfo() { return m_info; }
    
    void initialize(SurfaceInfo& info);

    // Checks if the preferred settings for format, color space and present mode are available.
    // If not, the function will set them to some fall back values.
    // Must be called right after the enumeration of the physical device.
    void checkSettingSupport(VkPhysicalDevice physicalDevice);

  private:
    void destroy();

    VkSurfaceKHR m_surface;
    SurfaceInfo m_info;

    bool m_created = false;
  };
}

#endif // SURFACE_HPP