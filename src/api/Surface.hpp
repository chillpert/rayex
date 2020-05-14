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

    void create(VkInstance instance, std::shared_ptr<Window> window);
    void destroy(VkInstance instance);

    // Queries the basic capabilitites of the surface, needed in order to create a swapchain.
    // Returns the optimal surface capabilities.
    VkSurfaceCapabilitiesKHR getCapabilitites(VkPhysicalDevice physicalDevice);
    // Checks if the physical device supports a surface.
    void checkPhysicalDeviceSupport(VkPhysicalDevice physicalDevice, uint32_t* queueFamilyIndex);

  private:
    // First checks if the physical device supports the prefered surface format and second if the surface supports it as well.
    void evaluateFormat(VkPhysicalDevice physicalDevice);
    void evaluatePresentMode(VkPhysicalDevice physicalDevice);

    VkSurfaceKHR surface;

    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
  };
}

#endif // SURFACE_HPP