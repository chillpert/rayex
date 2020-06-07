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

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface = m_info.surface;

    // Add another image so that the application does not have to wait for the driver before another image can be acquired.
    uint32_t minImageCount = m_info.surfaceCapabilities.minImageCount + 1;

    if (m_info.surfaceCapabilities.maxImageCount == 0)
      RX_ERROR("The surface does not support any images for a swap chain.");

    // If the preferred image count is exceeding the supported amount then use the maximum amount of images supported by the surface.
    if (minImageCount > m_info.surfaceCapabilities.maxImageCount)
      minImageCount = m_info.surfaceCapabilities.maxImageCount;

    createInfo.minImageCount = minImageCount;
    createInfo.imageFormat = m_info.surfaceFormat;
    createInfo.imageColorSpace = m_info.surfaceColorSpace;
    createInfo.preTransform = m_info.surfaceCapabilities.currentTransform;
    
    // Prefer opaque bit over any other composite alpha value.
    createInfo.compositeAlpha = 
      m_info.surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eOpaque ? vk::CompositeAlphaFlagBitsKHR::eOpaque :
      m_info.surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
      m_info.surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
      vk::CompositeAlphaFlagBitsKHR::eInherit;

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
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    if (m_info.queueFamilyIndices.size() > 1)
    {
      createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
      createInfo.queueFamilyIndexCount = static_cast<uint32_t>(m_info.queueFamilyIndices.size());
      createInfo.pQueueFamilyIndices = m_info.queueFamilyIndices.data();
    }
    else
      createInfo.imageSharingMode = vk::SharingMode::eExclusive;

    createInfo.presentMode = m_info.surfacePresentMode;

    m_swapchain = m_info.device.createSwapchainKHR(createInfo);
    if (!m_swapchain)
      RX_ERROR("Failed to create swapchain.");
    
    // Get swapchain images.
    m_info.images = m_info.device.getSwapchainImagesKHR(m_swapchain);
  }

  void Swapchain::destroy()
  {
    m_info.device.destroySwapchainKHR(m_swapchain);
    m_swapchain = nullptr;
  }

  void Swapchain::acquireNextImage(vk::Semaphore semaphore, vk::Fence fence, uint32_t* imageIndex)
  {
    m_info.device.acquireNextImageKHR(m_swapchain, UINT64_MAX, semaphore, fence, imageIndex);
  }

  vk::Format getSupportedDepthFormat(vk::PhysicalDevice physicalDevice)
  {
    std::vector<vk::Format> candidates{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };
    return Image::findSupportedFormat(physicalDevice, candidates, vk::FormatFeatureFlagBits::eDepthStencilAttachment, vk::ImageTiling::eOptimal);
  }
}