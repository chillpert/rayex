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
      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(physicalDevices[i], &props);

      if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      {
        physicalDevice = physicalDevices[i]; // TODO: Do proper device scoring!
        VK_LOG("GPU: " << props.deviceName);
        return;
      }
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

    bool found_VK_KHR_get_physical_device_properties2 = false;
    bool found_VK_KHR_get_memory_requirements2 = false;
    bool found_VK_EXT_descriptor_indexing = false;
    bool found_VK_KHR_buffer_device_address = false;
    bool found_VK_KHR_deferred_host_operations = false;
    bool found_VK_KHR_pipeline_library = false;
    bool found_VK_KHR_ray_tracing = false;

    uint32_t physicalDeviceExtensionCount;
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(physicalDevice, "VK_LAYER_KHRONOS_validation", &physicalDeviceExtensionCount, nullptr), "Failed to enumerate physical device extensions");

    std::vector<VkExtensionProperties> physicalDeviceExtensions(physicalDeviceExtensionCount);
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(physicalDevice, "VK_LAYER_KHRONOS_validation", &physicalDeviceExtensionCount, physicalDeviceExtensions.data()), "Failed to enumerate physical device extensions");

    // Iterates over all enumerated physical device extensions to see if they are available.
    for (const auto& physicalDeviceExtension : physicalDeviceExtensions)
    {
      for (auto& requiredphysicalDeviceExtension : requiredExtensions)
      {
        if (strcmp(physicalDeviceExtension.extensionName, requiredphysicalDeviceExtension.first))
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
}