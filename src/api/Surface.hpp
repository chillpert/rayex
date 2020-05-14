#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "window/Window.hpp"

namespace RX
{
  class Surface
  {
  public:
    Surface();

    inline VkSurfaceKHR get() { return surface; }
    
    VkSurfaceFormatKHR& getFormat(VkPhysicalDevice physicalDevice = VK_NULL_HANDLE);
    VkPresentModeKHR& getPresentMode(VkPhysicalDevice physicalDevice = VK_NULL_HANDLE);
    VkSurfaceCapabilitiesKHR& getCapabilitites(VkPhysicalDevice physicalDevice = VK_NULL_HANDLE);

    void create(VkInstance instance, std::shared_ptr<Window> window);
    void destroy(VkInstance instance);

  private:
    // First checks if the physical device supports the prefered surface format and second if the surface supports it as well.
    void evaluateFormat(VkPhysicalDevice physicalDevice);
    void evaluatePresentMode(VkPhysicalDevice physicalDevice);
    void evaluateCapabilities(VkPhysicalDevice physicalDevice);

    VkSurfaceKHR surface;

    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkSurfaceCapabilitiesKHR surfaceCapabilitites;
  };
}

#endif // SURFACE_HPP