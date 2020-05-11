#include "Swapchain.hpp"

namespace RX
{
  void Swapchain::create(VkPhysicalDevice physicalDevice, VkDevice device, Surface surface, std::shared_ptr<Window> window, uint32_t* familyIndex)
  {
    surface.checkFormatSupport(physicalDevice);
    surface.checkPhysicalDeviceSupport(physicalDevice, familyIndex);
    auto surfaceCapabilities = surface.getCapabilitites(physicalDevice);

    int width, height;
    window->getWindowSize(&width, &height);

    VkSwapchainCreateInfoKHR createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface.get();
    createInfo.minImageCount = 2;
    createInfo.imageFormat = surface.getFormat().format;
    createInfo.imageColorSpace = surface.getFormat().colorSpace;
    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.imageExtent.width = static_cast<uint32_t>(width);
    createInfo.imageExtent.height = static_cast<uint32_t>(height);
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = familyIndex;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    
    VK_ASSERT(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain), "Failed to create swapchain");
  }

  void Swapchain::destroy(VkDevice device)
  {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  }
}