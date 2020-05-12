#include "Device.hpp"

namespace RX
{
  void Device::create(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t* queueFamilyIndex)
  {
    *queueFamilyIndex = 0;

    uint32_t queueFamilyPropertyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

    // Iterates over all available queue families and find one that supports the graphics bit.
    for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i)
    {
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        *queueFamilyIndex = i;
        break;
      }
    }

    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueInfo.queueFamilyIndex = *queueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;

    std::vector<const char*> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VK_ASSERT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device), "Failed to create device");
  }

  void Device::destroy()
  {
    vkDestroyDevice(device, nullptr);
  }
}