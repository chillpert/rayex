#include "Surface.hpp"

namespace RX
{
  Surface::Surface() :
    BaseComponent("Surface"),
    surface(VK_NULL_HANDLE)
  {
    // Define prefered surface formats.
    surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
  }

  Surface::~Surface()
  {
    destroy();
  }

  VkSurfaceFormatKHR& Surface::getFormat(VkPhysicalDevice physicalDevice)
  {
    static bool checked = false;

    if (physicalDevice == VK_NULL_HANDLE && !checked)
      RX_ERROR("Surface::getFormat was called for the first time but without passing the pyhsical device to the function.");

    if (!checked)
    {
      evaluateFormat(physicalDevice);
      checked = true;
    }

    return surfaceFormat;
  }

  VkPresentModeKHR& Surface::getPresentMode(VkPhysicalDevice physicalDevice)
  {
    static bool checked = false;

    if (physicalDevice == VK_NULL_HANDLE && !checked)
      RX_ERROR("Surface::getPresentMode was called for the first time but without passing the pyhsical device to the function.");

    if (!checked)
    {
      evaluatePresentMode(physicalDevice);
      checked = true;
    }

    return presentMode;
  }

  VkSurfaceCapabilitiesKHR& Surface::getCapabilitites(VkPhysicalDevice physicalDevice)
  {
    evaluateCapabilities(physicalDevice);

    return surfaceCapabilitites;
  }

  void Surface::initialize(VkInstance instance, std::shared_ptr<Window> window)
  {
    m_instance = instance;

    surface = window->createSurface(instance);
    RX_INITIALIZATION_CALLBACK;
  }

  void Surface::destroy()
  {
    RX_ASSERT_DESTRUCTION;
    vkDestroySurfaceKHR(m_instance, surface, nullptr);
  }

  void Surface::evaluateFormat(VkPhysicalDevice physicalDevice)
  {
    RX_ASSERT_INITIALIZED("evaluateFormat");

    VkFormatProperties formatProperties{ };
    vkGetPhysicalDeviceFormatProperties(physicalDevice, surfaceFormat.format, &formatProperties); // TODO: Check if this color format is supported

    uint32_t surfaceFormatCounts;
    VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCounts, nullptr), "Failed to query physical device surface formats");

    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCounts);
    VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCounts, surfaceFormats.data()), "Failed to query physical device surface formats");

    for (const auto& iter : surfaceFormats)
    {
      if (iter.format == surfaceFormat.format && iter.colorSpace == surfaceFormat.colorSpace)
        return;
    }

    // If the prefered format and color space are not available, fall back.
    surfaceFormat.format = surfaceFormats[0].format;
    surfaceFormat.colorSpace = surfaceFormats[0].colorSpace;
  }

  void Surface::evaluatePresentMode(VkPhysicalDevice physicalDevice)
  {
    RX_ASSERT_INITIALIZED("evaluatePresentMode");

    uint32_t presentModeCount;
    VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr), "Failed to get physical device surface present modes");

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()), "Failed to get physical device surface present modes");
  
    for (const auto& mode : presentModes)
    {
      if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
      {
        presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        return;
      }
    }

    presentMode = VK_PRESENT_MODE_FIFO_KHR;
  }

  void Surface::evaluateCapabilities(VkPhysicalDevice physicalDevice)
  {
    RX_ASSERT_INITIALIZED("evaluateCapabilities");
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilitites);
  }
}