#ifndef VK_UTILS_HPP
#define VK_UTILS_HPP

#include "pch/stdafx.hpp"
#include "window/Window.hpp"
#include "api/VkShader.hpp"

namespace RX
{
  VkInstance createInstance(std::shared_ptr<Window> window);

  VkPhysicalDevice pickPhysicalDevice(VkInstance instance);

  VkDevice createDevice(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t* familyIndex);

  VkSwapchainKHR createSwapChain
  (
    VkPhysicalDevice physicalDevice, 
    VkDevice device, 
    VkSurfaceKHR surface, 
    std::shared_ptr<Window> window, 
    uint32_t* familyIndex, 
    VkFormat* format
  );

  VkSemaphore createSemaphore(VkDevice device);

  VkCommandPool createCommandPool(VkDevice device, uint32_t* familyIndex);

  VkRenderPass createRenderPass(VkDevice device,  VkFormat format);

  VkPipeline createPipeline
  (
    VkDevice device, 
    VkRenderPass renderPass,
    std::shared_ptr<Window> window,
    std::shared_ptr<VkShader> vertex, 
    std::shared_ptr<VkShader> fragment
  );

  VkFramebuffer createFramebuffer
  (
    VkDevice device, 
    VkRenderPass renderPass, 
    VkImageView imageView, 
    std::shared_ptr<Window> window
  );

  VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);
}

#endif // VK_UTILS_HPP