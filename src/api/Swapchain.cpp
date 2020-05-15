#include "Swapchain.hpp"

namespace RX
{
  Swapchain::Swapchain() :
    BaseComponent("Swapchain") { }

  void Swapchain::initialize(VkPhysicalDevice physicalDevice, VkDevice device, Surface surface, std::shared_ptr<Window> window, QueueManager& queueManager)
  {
    auto surfaceCapabilities = surface.getCapabilitites(physicalDevice);

    VkSwapchainCreateInfoKHR createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface.get();

    // Add another image so that the application does not have to wait for the driver before another image can be acquired.
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;

    if (surfaceCapabilities.maxImageCount == 0)
      RX_ERROR("The surface does not support any images for a swap chain");

    // If the preferred image count is exceeding the supported amount then use the maximum amount of images supported by the surface.
    if (imageCount > surfaceCapabilities.maxImageCount && surfaceCapabilities.maxImageCount > 0)
      imageCount = surfaceCapabilities.maxImageCount;

    createInfo.minImageCount = imageCount;
    
    createInfo.imageFormat = surface.getFormat(physicalDevice).format;
    createInfo.imageColorSpace = surface.getFormat(physicalDevice).colorSpace;
    createInfo.preTransform = surfaceCapabilities.currentTransform;
    
    // Prefer opaque bit over any other composite alpha value.
    createInfo.compositeAlpha = 
      surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR ? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR :
      surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR ? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR :
      surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR ? VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR :
      VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;

    // Handle the swap chain image extent.
    if (surfaceCapabilities.currentExtent.width != 0xFFFFFFFF)
    {
      // The surface size will be determined by the extent of a swapchain targeting the surface.
      extent = surfaceCapabilities.currentExtent;
    }
    else
    {
      // Clamp width and height.
      extent = window->getExtent();

      uint32_t width_t = extent.width;
      if (surfaceCapabilities.maxImageExtent.width < extent.width)
        width_t = surfaceCapabilities.maxImageExtent.width;

      uint32_t height_t = extent.height;
      if (surfaceCapabilities.maxImageExtent.height < extent.height)
        height_t = surfaceCapabilities.maxImageExtent.height;

      extent.width = width_t;
      if (surfaceCapabilities.minImageExtent.width > width_t)
        extent.width = surfaceCapabilities.minImageExtent.width;

      extent.height = height_t;
      if (surfaceCapabilities.minImageExtent.height > height_t)
        extent.height = surfaceCapabilities.minImageExtent.height;      
    }

    createInfo.imageExtent = extent;

    if (surfaceCapabilities.maxImageArrayLayers < 1)
      RX_ERROR("The surface does not support a single array layer");

    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto graphicsIndex = queueManager.getGraphicsIndex();
    auto presentIndex = queueManager.getPresentIndex();
    std::vector<uint32_t> queueFamilyIndices = { graphicsIndex, presentIndex };

    if (graphicsIndex != presentIndex)
    {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
      createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }
    else
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    createInfo.presentMode = surface.getPresentMode(physicalDevice);
    
    VK_ASSERT(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain), "Failed to create swapchain");

    initializationCallback();
  }

  void Swapchain::destroy(VkDevice device)
  {
    assertDestruction();
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  }

  void Swapchain::initializeImages(VkDevice device)
  {
    assertInitialized("createImages");

    uint32_t imageCount;
    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr), "Failed to get swap chain images");

    images.resize(imageCount);
    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()), "Failed to get swap chain images");
  }

  void Swapchain::initializeImageViews(VkDevice device, Surface surface)
  {
    assertInitialized("createImageViews");

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

  void Swapchain::initializeFramebuffers(VkDevice device, VkRenderPass renderPass, std::shared_ptr<Window> window)
  {
    assertInitialized("createFramebuffers");

    framebuffers.resize(imageViews.size());

    uint32_t imageCount = static_cast<uint32_t>(imageViews.size());

    int width, height;
    window->getSize(&width, &height);

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