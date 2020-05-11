#include "Surface.hpp"

namespace RX
{
  Surface::Surface() :
    surface(VK_NULL_HANDLE)
  {
    // Define prefered surface formats.
    surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
  }

  void Surface::create(VkInstance instance, std::shared_ptr<Window> window)
  {
    surface = window->createSurface(instance);
  }

  void Surface::destroy(VkInstance instance)
  {
    vkDestroySurfaceKHR(instance, surface, nullptr);
  }

  VkSurfaceCapabilitiesKHR Surface::getCapabilitites(VkPhysicalDevice physicalDevice)
  {
    VkSurfaceCapabilitiesKHR surfaceCapabilitites;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilitites);

    if (!(surfaceCapabilitites.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR))
      VK_ERROR("VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR is not supported on this device"); // TODO: make generic

    if (!(surfaceCapabilitites.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT))
      VK_ERROR("VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT is not supported on this device"); // TODO: make generic

    return surfaceCapabilitites;
  }

  void Surface::checkPhysicalDeviceSupport(VkPhysicalDevice physicalDevice, uint32_t* queueFamilyIndex)
  {
    VkBool32 supported = false;
    VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, *queueFamilyIndex, surface, &supported), "Failed to query pyhsical device surface support");

    if (supported == VK_FALSE)
      VK_ERROR("Physical device surface does not support WSI");
  }

  void Surface::checkFormatSupport(VkPhysicalDevice physicalDevice)
  {
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
}