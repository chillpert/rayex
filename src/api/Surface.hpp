#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "window/Window.hpp"

namespace RX
{
  class Surface
  {
  public:
    inline VkSurfaceKHR get() { return surface; }
    inline VkSurfaceFormatKHR getFormat() { return surfaceFormat; }

    void create(VkInstance instance, std::shared_ptr<Window> window);
    void destroy(VkInstance instance);

    // Queries the basic capabilitites of the surface, needed in order to create a swapchain.
    // Returns the optimal surface capabilities.
    VkSurfaceCapabilitiesKHR getCapabilitites(VkPhysicalDevice physicalDevice);
    // Checks if the physical device supports a surface.
    void checkPhysicalDeviceSupport(VkPhysicalDevice physicalDevice, uint32_t* queueFamilyIndex);
    // First checks if the physical device supports the prefered surface format and second if the surface supports it as well.
    void checkFormatSupport(VkPhysicalDevice physicalDevice);

  private:
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
  };
}

#endif // SURFACE_HPP