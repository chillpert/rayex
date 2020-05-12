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
    createInfo.minImageCount = 2; // Remark: Maybe triple buffering at some point 
    createInfo.imageFormat = surface.getFormat().format;
    createInfo.imageColorSpace = surface.getFormat().colorSpace;
    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // TODO: check if available
    createInfo.imageExtent.width = static_cast<uint32_t>(width);
    createInfo.imageExtent.height = static_cast<uint32_t>(height);
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = familyIndex;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // TODO
    
    VK_ASSERT(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain), "Failed to create swapchain");
  }

  void Swapchain::destroy(VkDevice device)
  {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  }

  void Swapchain::createImages(VkDevice device)
  {
    uint32_t imageCount;
    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr), "Failed to get swap chain images");

    images.resize(imageCount);
    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()), "Failed to get swap chain images");
  }

  void Swapchain::createImageViews(VkDevice device, Surface surface)
  {
    imageViews.resize(images.size());

    uint32_t imageCount = static_cast<uint32_t>(images.size());

    for (uint32_t i = 0; i < imageCount; ++i)
    {
      VkImageViewCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = images[i];
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format = surface.getFormat().format;
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel = 0;
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;

      VK_ASSERT(vkCreateImageView(device, &createInfo, nullptr, &imageViews[i]), "Failed to create image view");
    }
  }

  void Swapchain::createFramebuffers(VkDevice device, VkRenderPass renderPass, std::shared_ptr<Window> window)
  {
    framebuffers.resize(imageViews.size());

    uint32_t imageCount = static_cast<uint32_t>(imageViews.size());

    int width, height;
    window->getWindowSize(&width, &height);

    for (uint32_t i = 0; i < imageCount; ++i)
    {
      VkFramebufferCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      createInfo.renderPass = renderPass;
      createInfo.attachmentCount = 1;
      createInfo.pAttachments = &imageViews[i];
      createInfo.width = static_cast<uint32_t>(width);
      createInfo.height = static_cast<uint32_t>(height);

      createInfo.layers = 1;

      VK_ASSERT(vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffers[i]), "Failed to create frame buffer");
    }
  }
}