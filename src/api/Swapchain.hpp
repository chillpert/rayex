#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "window/Window.hpp"

namespace RX
{
  class Swapchain
  {
  public:
    inline VkSwapchainKHR get() { return swapchain; }
    
    // MOVE THIS TO SURFACE CLASS?
    inline VkSurfaceFormatKHR getSurfaceFormat() { return surfaceFormat; }

    void create(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, std::shared_ptr<Window> window, uint32_t* familyIndex);
    void destroy(VkDevice device);

  private:
    // Checks if the physical device supports a surface.
    void checkPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* familyIndex);
    // First checks if the physical device supports the prefered surface format and second if the surface supports it as well.
    void checkFormatSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    // Queries the basic capabilitites of the surface, needed in order to create a swapchain.
    // Returns SurfaceCapabilities optimal 
    VkSurfaceCapabilitiesKHR getSurfaceCapabilitites(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    VkSwapchainKHR swapchain;
    VkSurfaceFormatKHR surfaceFormat;
  };
}

#endif // SWAPCHAIN_HPP