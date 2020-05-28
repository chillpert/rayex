#include "Surface.hpp"

namespace RX
{
  Surface::~Surface()
  {
    destroy();
  }

  void Surface::initialize(SurfaceInfo& info)
  {
    m_info = info;

    m_surface = m_info.window->createSurface(m_info.instance);
  }

  void Surface::checkSettingSupport(VkPhysicalDevice physicalDevice)
  {
    // Get all surface capabilities.
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &m_capabilities);

    // Check a present mode.
    uint32_t presentModeCount;
    VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, nullptr), "Failed to get physical device surface present modes");

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, presentModes.data()), "Failed to get physical device surface present modes");

    for (const auto& mode : presentModes)
    {
      if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
      {
        m_info.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        return;
      }
    }

    m_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;

    // Check format and color space.
    VkFormatProperties formatProperties{ };
    vkGetPhysicalDeviceFormatProperties(physicalDevice, m_info.format, &formatProperties); // TODO: Check if this color format is supported

    uint32_t surfaceFormatCounts;
    VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &surfaceFormatCounts, nullptr), "Failed to query physical device surface formats");

    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCounts);
    VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &surfaceFormatCounts, surfaceFormats.data()), "Failed to query physical device surface formats");

    for (const auto& iter : surfaceFormats)
    {
      if (iter.format == m_info.format && iter.colorSpace == m_info.colorSpace)
        return;
    }

    // If the prefered format and color space are not available, fall back.
    m_info.format = surfaceFormats[0].format;
    m_info.colorSpace = surfaceFormats[0].colorSpace;
  }

  void Surface::destroy()
  {
    vkDestroySurfaceKHR(m_info.instance, m_surface, nullptr);
  }
}