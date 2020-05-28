#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "window/Window.hpp"

namespace RX
{
  struct Components
  {
    std::shared_ptr<Window> window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;

    VkSwapchainKHR swapchain;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue transferQueue;
    uint32_t graphicsFamilyIndex;
    uint32_t presentFamilyIndex;
    uint32_t transferFamilyIndex;
  };
}

#endif // COMPONENTS_HPP