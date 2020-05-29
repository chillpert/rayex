#include "Swapchain.hpp"

namespace RX
{
  Swapchain::~Swapchain()
  {
    destroy();
  }

  void Swapchain::initialize(SwapchainInfo& info)
  {
    m_info = info;

    VkSwapchainCreateInfoKHR createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_info.surface;

    // Add another image so that the application does not have to wait for the driver before another image can be acquired.
    uint32_t imageCount = m_info.surfaceCapabilities.minImageCount + 1;

    if (m_info.surfaceCapabilities.maxImageCount == 0)
      RX_ERROR("The surface does not support any images for a swap chain.");

    // If the preferred image count is exceeding the supported amount then use the maximum amount of images supported by the surface.
    if (imageCount > m_info.surfaceCapabilities.maxImageCount && m_info.surfaceCapabilities.maxImageCount > 0)
      imageCount = m_info.surfaceCapabilities.maxImageCount;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = m_info.surfaceFormat;
    createInfo.imageColorSpace = m_info.surfaceColorSpace;
    createInfo.preTransform = m_info.surfaceCapabilities.currentTransform;
    
    // Prefer opaque bit over any other composite alpha value.
    createInfo.compositeAlpha = 
      m_info.surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR ? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR :
      m_info.surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR ? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR :
      m_info.surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR ? VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR :
      VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;

    // Handle the swap chain image extent.
    if (m_info.surfaceCapabilities.currentExtent.width != UINT32_MAX)
    {
      // The surface size will be determined by the extent of a swapchain targeting the surface.
      m_info.extent = m_info.surfaceCapabilities.currentExtent;
    }
    else
    {
      // Clamp width and height.
      m_info.extent = m_info.window->getExtent();

      uint32_t width_t = m_info.extent.width;
      if (m_info.surfaceCapabilities.maxImageExtent.width < m_info.extent.width)
        width_t = m_info.surfaceCapabilities.maxImageExtent.width;

      uint32_t height_t = m_info.extent.height;
      if (m_info.surfaceCapabilities.maxImageExtent.height < m_info.extent.height)
        height_t = m_info.surfaceCapabilities.maxImageExtent.height;

      m_info.extent.width = width_t;
      if (m_info.surfaceCapabilities.minImageExtent.width > width_t)
        m_info.extent.width = m_info.surfaceCapabilities.minImageExtent.width;

      m_info.extent.height = height_t;
      if (m_info.surfaceCapabilities.minImageExtent.height > height_t)
        m_info.extent.height = m_info.surfaceCapabilities.minImageExtent.height;
    }

    createInfo.imageExtent = m_info.extent;

    if (m_info.surfaceCapabilities.maxImageArrayLayers < 1)
      RX_ERROR("The surface does not support a single array layer.");

    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (m_info.queueFamilyIndices.size() > 1)
    {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = static_cast<uint32_t>(m_info.queueFamilyIndices.size());
      createInfo.pQueueFamilyIndices = m_info.queueFamilyIndices.data();
    }
    else
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    createInfo.presentMode = m_info.surfacePresentMode;

    VK_CREATE(vkCreateSwapchainKHR(m_info.device, &createInfo, nullptr, &m_swapchain), "swapchain");

    // Get swapchain images.
    uint32_t imageCount_t;
    VK_ASSERT(vkGetSwapchainImagesKHR(m_info.device, m_swapchain, &imageCount_t, nullptr), "Failed to get swap chain images.");

    m_info.images.resize(static_cast<size_t>(imageCount_t));
    VK_ASSERT(vkGetSwapchainImagesKHR(m_info.device, m_swapchain, &imageCount_t, m_info.images.data()), "Failed to get swap chain images.");
  }

  void Swapchain::destroy()
  {
    VK_DESTROY(vkDestroySwapchainKHR(m_info.device, m_swapchain, nullptr), "swapchain");
    m_swapchain = VK_NULL_HANDLE;
  }

  void Swapchain::acquireNextImage(VkSemaphore semaphore, VkFence fence, uint32_t* imageIndex)
  {
    VK_ASSERT(vkAcquireNextImageKHR(m_info.device, m_swapchain, UINT64_MAX, semaphore, fence, imageIndex), "Failed to acquire next image from swapchain");
  }

  VkFormat getSupportedDepthFormat(VkPhysicalDevice physicalDevice)
  {
    std::vector<VkFormat> candidates{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    return Image::findSupportedFormat(physicalDevice, candidates, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL);
  }
}