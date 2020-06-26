#include "Device.hpp"

namespace RX
{
  Device::Device(DeviceInfo& info)
  {
    init(info);
  }

  Device::Device(DeviceInfo&& info)
  {
    init(info);
  }

  void Device::init(DeviceInfo& info)
  {
    m_info = info;

    m_info.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    checkExtensionSupport();

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    std::vector<std::shared_ptr<std::vector<float>>> priorities(m_info.queueFamilies.size());

    // Note: This process takes about half a second on a Ryzen 5 3600x + RTX 2070. Maybe it's not worth creating all queues available for a path tracer.
    uint32_t index = 0;
    for (QueueFamily& queueFamily : m_info.queueFamilies)
    {
      priorities[index] = std::make_shared<std::vector<float>>(queueFamily.queues.size());

      vk::DeviceQueueCreateInfo queueCreateInfo;
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

    vk::PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    vk::PhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures;
    bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;

    vk::PhysicalDeviceFeatures2 deviceFeatures2;
    deviceFeatures2.pNext = &bufferDeviceAddressFeatures;
    deviceFeatures2.features = deviceFeatures;

    vk::DeviceCreateInfo createInfo;
    createInfo.pNext = &deviceFeatures2;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = nullptr; // This must be NULL because the VkDeviceCreateInfo::pNext chain includes VkPhysicalDeviceFeatures2.
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_info.extensions.size());
    createInfo.ppEnabledExtensionNames = m_info.extensions.data();

    m_device = m_info.physicalDevice.createDeviceUnique(createInfo);

    if (!m_device)
      RX_ERROR("Failed to create logical device.");
  }

  void Device::init(DeviceInfo&& info)
  {
    init(info);
  }

  void Device::waitIdle()
  {
    m_device->waitIdle();
  }

  void Device::checkExtensionSupport() const
  {
    // Stores the name of the extension and a bool that tells if they were found or not.
    std::map<const char*, bool> requiredExtensions;

    for (const auto& extension : m_info.extensions)
      requiredExtensions.emplace(extension, false);

    std::vector<vk::ExtensionProperties> physicalDeviceExtensions = m_info.physicalDevice.enumerateDeviceExtensionProperties();

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