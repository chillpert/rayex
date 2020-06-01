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

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::vector<std::shared_ptr<std::vector<float>>> priorities(m_info.queueFamilies.size());

    // Note: This process takes about half a second on a Ryzen 5 3600x + RTX 2070. Maybe it's not worth creating all queues available for a path tracer.
    uint32_t index = 0;
    for (QueueFamily& queueFamily : m_info.queueFamilies)
    {
      priorities[index] = std::make_shared<std::vector<float>>(queueFamily.queues.size());

      VkDeviceQueueCreateInfo queueCreateInfo{ };
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily.index;
      queueCreateInfo.queueCount = static_cast<uint32_t>(queueFamily.queues.size());
      //queueCreateInfo.flags = VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT;

      for (size_t i = 0; i < queueFamily.queues.size(); ++i)
        priorities[index]->at(i) = queueFamily.queues[i]->getPriority();

      RX_ASSERT((queueFamily.queues.size() == priorities[index]->size()), "Amount of queues in family index are not equal to the specified amount of priorities.");

      queueCreateInfo.pQueuePriorities = priorities[index]->data();
      queueCreateInfos.push_back(queueCreateInfo);

      ++index;
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

    VK_CREATE(vkCreateDevice(m_info.physicalDevice, &createInfo, nullptr, &m_device), "device");
  }

  void Device::destroy()
  {
    VK_DESTROY(vkDestroyDevice(m_device, nullptr), "device");
    m_device = VK_NULL_HANDLE;
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
        RX_LOG("Added device extension: " << requiredphysicalDeviceExtension.first);
    }
  }
}