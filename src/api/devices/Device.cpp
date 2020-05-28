#include "Device.hpp"

namespace RX
{
  Device::~Device()
  {
    destroy();
  }

  void Device::initialize(DeviceInfo& info)
  {
    m_info = info;

    m_info.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    checkExtensionSupport();

    const float queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    for (uint32_t queueFamilyIndex : m_info.queueFamilyIndices)
    {
      VkDeviceQueueCreateInfo queueCreateInfo{ };
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{ };
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkPhysicalDeviceFeatures2 deviceFeatures2{ };
    deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    deviceFeatures2.pNext = nullptr;
    deviceFeatures2.features = deviceFeatures;

    VkDeviceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = &deviceFeatures2;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = nullptr; // This must be NULL because the VkDeviceCreateInfo::pNext chain includes VkPhysicalDeviceFeatures2.
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_info.extensions.size());
    createInfo.ppEnabledExtensionNames = m_info.extensions.data();

    VK_ASSERT(vkCreateDevice(m_info.physicalDevice, &createInfo, nullptr, &m_device), "Failed to create device.");
  }

  void Device::destroy()
  {
    vkDestroyDevice(m_device, nullptr);
  }

  void Device::waitIdle()
  {
    vkDeviceWaitIdle(m_device);
  }

  void Device::checkExtensionSupport() const
  {
    // Stores the name of the extension and a bool that tells if they were found or not.
    std::map<const char*, bool> requiredExtensions;

    for (const auto& extension : m_info.extensions)
      requiredExtensions.emplace(extension, false);

    uint32_t physicalDeviceExtensionCount;
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(m_info.physicalDevice, NULL, &physicalDeviceExtensionCount, nullptr), "Failed to enumerate physical device extensions");

    std::vector<VkExtensionProperties> physicalDeviceExtensions(physicalDeviceExtensionCount);
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(m_info.physicalDevice, NULL, &physicalDeviceExtensionCount, physicalDeviceExtensions.data()), "Failed to enumerate physical device extensions");

    // Iterates over all enumerated physical device extensions to see if they are available.
    for (const auto& physicalDeviceExtension : physicalDeviceExtensions)
    {
      for (auto& requiredphysicalDeviceExtension : requiredExtensions)
      {
        if (strcmp(physicalDeviceExtension.extensionName, requiredphysicalDeviceExtension.first) == 0)
          requiredphysicalDeviceExtension.second = true;
      }
    }

    // Give feedback on the previous operations.
    for (const auto& requiredphysicalDeviceExtension : requiredExtensions)
    {
      if (!requiredphysicalDeviceExtension.second)
        RX_ERROR("Missing physical device extension: " + requiredphysicalDeviceExtension.first + ". Have you tried installing the NVIDIA Beta drivers?");
      else
        VK_LOG("Added device extension: " << requiredphysicalDeviceExtension.first);
    }
  }
}