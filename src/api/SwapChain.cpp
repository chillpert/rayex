#include "api/SwapChain.hpp"
#include "api/Device.hpp"
#include "utils/VkUtils.hpp"

namespace RX
{
  VkSurfaceKHR* SwapChain::s_surface = VK_NULL_HANDLE;

  SwapChain::SwapChain(VkSurfaceKHR* surface, std::shared_ptr<Window> window) :
    m_logicalDevice(VK_NULL_HANDLE),
    m_swapChain(VK_NULL_HANDLE),
    m_swapChainExtent { },
    m_window(window)
  {
    s_surface = surface;
  }

  void SwapChain::createSwapChain(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice)
  {
    m_logicalDevice = logicalDevice;

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;

    // Sets the amount of images in the swap chain.
    // However, going with the minimum might result in waiting for the driver to complete internal operations.
    // This is why the amount of images is incremented by one.
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo { };
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = *s_surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // for post-processing might use VK_IMAGE_USAGE_TRANSFER_DST_BIT

    QueueFamilyIndices indices = Device::findQueueFamilies(*physicalDevice);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily)
    {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0; // Optional
      createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    Assert::vulkan(vkCreateSwapchainKHR(*m_logicalDevice, &createInfo, nullptr, &m_swapChain), "Failed to create swap chain");

    vkGetSwapchainImagesKHR(*m_logicalDevice, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(*m_logicalDevice, m_swapChain, &imageCount, m_swapChainImages.data());
  }

  void SwapChain::createImageViews()
  {
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++)
    {
      VkImageViewCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = m_swapChainImages[i];
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format = m_swapChainImageFormat;
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel = 0;
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;

      Assert::vulkan(vkCreateImageView(*m_logicalDevice, &createInfo, nullptr, &m_swapChainImageViews[i]), "Failed to create image views");
    }
  }

  void SwapChain::createFramebuffers(VkRenderPass* renderPass)
  {
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (size_t i = 0; i < m_swapChainImageViews.size(); ++i)
    {
      VkImageView attachments[] = { m_swapChainImageViews[i] };

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = *renderPass;
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = attachments;
      framebufferInfo.width = m_swapChainExtent.width;
      framebufferInfo.height = m_swapChainExtent.height;
      framebufferInfo.layers = 1;

      Assert::vulkan(vkCreateFramebuffer(*m_logicalDevice, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]), "Failed to create framebuffer");
    }
  }

  void SwapChain::destroySwapChain()
  {
    vkDestroySwapchainKHR(*m_logicalDevice, m_swapChain, nullptr);
  }

  void SwapChain::destroyImageView()
  {
    for (auto imageView : m_swapChainImageViews)
    {
      vkDestroyImageView(*m_logicalDevice, imageView, nullptr);
    }
  }

  void SwapChain::destroyFramebuffers()
  {
    for (auto framebuffer : m_swapChainFramebuffers)
    {
      vkDestroyFramebuffer(*m_logicalDevice, framebuffer, nullptr);
    }
  }

  SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device)
  {
    if (s_surface == VK_NULL_HANDLE)
    {
      Error::runtime("Can not query swap chain support, because the window surface has not been created yet", Error::API);
    }

    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *s_surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, *s_surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, *s_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, *s_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, *s_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }

  VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
  {
    for (const auto& availableFormat : availableFormats)
    {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        return availableFormat;
    }

    return availableFormats[0];
  }

  VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
  {
    for (const auto& availablePresentMode : availablePresentModes)
    {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        return availablePresentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
  {
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
      return capabilities.currentExtent;
    }
    else
    {
      VkExtent2D actualExtent = { m_window->getProperties().getWidth(), m_window->getProperties().getHeight() };

      actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
      actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

      return actualExtent;
    }
  }
}