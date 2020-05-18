#include "Swapchain.hpp"

namespace RX
{
  Swapchain::Swapchain() :
    BaseComponent("Swapchain") { }

  Swapchain::~Swapchain()
  {
    destroy();
  }

  void Swapchain::initialize(VkPhysicalDevice physicalDevice, VkDevice device, Surface& surface, std::shared_ptr<Window> window, Queues& queues)
  {
    m_device = device;

    auto surfaceCapabilities = surface.getCapabilitites(physicalDevice);

    VkSwapchainCreateInfoKHR createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface.get();

    // Add another image so that the application does not have to wait for the driver before another image can be acquired.
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;

    if (surfaceCapabilities.maxImageCount == 0)
      RX_ERROR("The surface does not support any images for a swap chain.");

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
      m_extent = surfaceCapabilities.currentExtent;
    }
    else
    {
      // Clamp width and height.
      m_extent = window->getExtent();

      uint32_t width_t = m_extent.width;
      if (surfaceCapabilities.maxImageExtent.width < m_extent.width)
        width_t = surfaceCapabilities.maxImageExtent.width;

      uint32_t height_t = m_extent.height;
      if (surfaceCapabilities.maxImageExtent.height < m_extent.height)
        height_t = surfaceCapabilities.maxImageExtent.height;

      m_extent.width = width_t;
      if (surfaceCapabilities.minImageExtent.width > width_t)
        m_extent.width = surfaceCapabilities.minImageExtent.width;

      m_extent.height = height_t;
      if (surfaceCapabilities.minImageExtent.height > height_t)
        m_extent.height = surfaceCapabilities.minImageExtent.height;      
    }

    createInfo.imageExtent = m_extent;

    if (surfaceCapabilities.maxImageArrayLayers < 1)
      RX_ERROR("The surface does not support a single array layer.");

    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto graphicsIndex = queues.getGraphicsIndex();
    auto presentIndex = queues.getPresentIndex();
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
    
    VK_ASSERT(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapchain), "Failed to create swapchain.");

    initializationCallback();
  }

  void Swapchain::destroy()
  {
    assertDestruction();
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
  }
}