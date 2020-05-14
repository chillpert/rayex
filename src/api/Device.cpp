#include "Device.hpp"

namespace RX
{
  void Device::create(VkPhysicalDevice physicalDevice, QueueManager& queueManager)
  {
    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<uint32_t> queueFamilyIndices = queueManager.getQueueFamilyIndices();

    for (uint32_t queueFamilyIndex : queueFamilyIndices)
    {
      VkDeviceQueueCreateInfo queueCreateInfo{ };
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{ };

    VkPhysicalDeviceFeatures2 deviceFeatures2{ };
    deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    deviceFeatures2.pNext = nullptr;
    deviceFeatures2.features = deviceFeatures;

    VkDeviceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = &deviceFeatures2;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    // This must be NULL because the VkDeviceCreateInfo::pNext chain includes VkPhysicalDeviceFeatures2.
    createInfo.pEnabledFeatures = nullptr;

    pushExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VK_ASSERT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device), "Failed to create device.");

    queueManager.retrieveAllQueueHandles(device);

    created = true;
  }

  void Device::destroy()
  {
    if (created)
    {
      vkDestroyDevice(device, nullptr);
      created = false;
    }

    VK_ERROR("Logical device was aready deleted or not created to begin with.");
  }

  void Device::pushExtension(const char* name)
  {
    if (created)
      VK_ERROR("The logical device was already created. Can not push another extension.");

    extensions.push_back(name);
  }
}