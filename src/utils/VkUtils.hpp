#ifndef VK_UTILS_HPP
#define VK_UTILS_HPP

#include "pch/stdafx.hpp"
#include "window/Window.hpp"

namespace RX
{
  VkInstance createInstance(std::shared_ptr<Window> window);

  // Retrieves all physical devices and returns the first discrete GPU that was found
  VkPhysicalDevice pickPhysicalDevice(VkInstance instance);

  VkDevice createDevice(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t* familyIndex);

  VkSwapchainKHR createSwapChain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, std::shared_ptr<Window> window, uint32_t* familyIndex);

  VkSemaphore createSemaphore(VkDevice device);
}

#endif // VK_UTILS_HPP