#include "PhysicalDevice.hpp"

namespace RX
{
  void PhysicalDevice::create(VkInstance instance)
  {
    uint32_t physicalDeviceCount = 0;
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr), "Failed to enumerate physical devices");

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()), "Failed to enumerate physical devices");

    for (uint32_t i = 0; i < physicalDeviceCount; ++i)
    {
      
    }

    // Any non-discrete GPU will not have enough performance to do path tracing at all.
    VK_ERROR("No discrete GPU with Vulkan support available");
  }

  void PhysicalDevice::checkExtensionSupport(const std::vector<const char*>& extensions) const
  {
    // Stores the name of the extension and a bool that tells if they were found or not.
    std::map<const char*, bool> requiredExtensions;

    for (const auto& extension : extensions)
      requiredExtensions.emplace(extension, false);

    uint32_t physicalDeviceExtensionCount;
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &physicalDeviceExtensionCount, nullptr), "Failed to enumerate physical device extensions");

    std::vector<VkExtensionProperties> physicalDeviceExtensions(physicalDeviceExtensionCount);
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &physicalDeviceExtensionCount, physicalDeviceExtensions.data()), "Failed to enumerate physical device extensions");
      
    // Iterates over all enumerated physical device extensions to see if they are available.
    for (const auto& physicalDeviceExtension : physicalDeviceExtensions)
    {
      for (auto& requiredphysicalDeviceExtension : requiredExtensions)
      {
        if (strcmp(physicalDeviceExtension.extensionName, requiredphysicalDeviceExtension.first) == 0)
          requiredphysicalDeviceExtension.second = true;
      }
    }

    // Give feedback on the previous operations
    for (const auto& requiredphysicalDeviceExtension : requiredExtensions)
    {
      if (!requiredphysicalDeviceExtension.second)
        VK_ERROR("Missing physical device extension: " + std::string(requiredphysicalDeviceExtension.first));
    }
  }

  size_t PhysicalDevice::evaluate(VkPhysicalDevice device)
  {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physicalDevice, &props);




    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
      physicalDevice = physicalDevice; // TODO: Do proper device scoring!
      VK_LOG("GPU: " << props.deviceName);
      return;
    }

    return size_t();
  }
}