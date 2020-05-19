#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "BaseComponent.hpp"
#include "window/Window.hpp"

namespace RX
{
  class Surface : public BaseComponent
  {
  public:
    Surface();
    ~Surface();

    inline VkSurfaceKHR get() { return surface; }
    
    VkSurfaceFormatKHR& getFormat(VkPhysicalDevice physicalDevice = VK_NULL_HANDLE);
    VkPresentModeKHR& getPresentMode(VkPhysicalDevice physicalDevice = VK_NULL_HANDLE);
    VkSurfaceCapabilitiesKHR& getCapabilitites(VkPhysicalDevice physicalDevice);

    void initialize(VkInstance instance, std::shared_ptr<Window> window);
    void destroy();

  private:
    // First checks if the physical device supports the prefered surface format and second if the surface supports it as well.
    void evaluateFormat(VkPhysicalDevice physicalDevice);
    void evaluatePresentMode(VkPhysicalDevice physicalDevice);
    void evaluateCapabilities(VkPhysicalDevice physicalDevice);

    VkSurfaceKHR surface; // TODO: rename

    VkSurfaceFormatKHR surfaceFormat; // TODO: rename
    VkPresentModeKHR presentMode; // TODO: rename
    VkSurfaceCapabilitiesKHR surfaceCapabilitites; // TODO: rename

    VkInstance m_instance;
  };
}

#endif // SURFACE_HPP