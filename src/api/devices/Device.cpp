#include "Device.hpp"
#include "Components.hpp"

namespace RX
{
  Device::Device(const std::vector<QueueFamily>& queueFamilies, std::vector<const char*>& extensions, bool initialize)
  {
    if (initialize)
      init(queueFamilies, extensions);
  }

  void Device::init(const std::vector<QueueFamily>& queueFamilies, std::vector<const char*>& extensions)
  {
    extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    checkExtensionSupport(extensions);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    std::vector<std::shared_ptr<std::vector<float>>> priorities(queueFamilies.size());

    // Note: This process takes about half a second on a Ryzen 5 3600x + RTX 2070. Maybe it's not worth creating all queues available.
    uint32_t index = 0;
    for (const auto& queueFamily : queueFamilies)
    {
      priorities[index] = std::make_shared<std::vector<float>>(queueFamily.queues.size());

      for (size_t i = 0; i < queueFamily.queues.size(); ++i)
        priorities[index]->at(i) = queueFamily.queues[i]->getPriority();

      RX_ASSERT((queueFamily.queues.size() == priorities[index]->size()), "Amount of queues in family index are not equal to the specified amount of priorities.");

      vk::DeviceQueueCreateInfo queueCreateInfo{
        { },                                              // flags
        queueFamily.index,                                // queueFamilyIndex
        static_cast<uint32_t>(queueFamily.queues.size()), // queueCount
        priorities[index]->data()                         // pQueuePriorties
      };

      queueCreateInfos.push_back(queueCreateInfo);

      ++index;
    }

    vk::PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    vk::PhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures;
    bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;

    vk::PhysicalDeviceFeatures2 deviceFeatures2{ deviceFeatures };
    deviceFeatures2.setPNext(&bufferDeviceAddressFeatures);

    vk::DeviceCreateInfo createInfo{
      { },                                            // flags
      static_cast<uint32_t>(queueCreateInfos.size()), // queueCreateInfoCount
      queueCreateInfos.data(),                        // pQueueCreateInfos
      0,                                              // enabledLayerCount
      nullptr,                                        // ppEnabledLayerNames
      static_cast<uint32_t>(extensions.size()),       // enabledExtensionCount
      extensions.data(),                              // ppEnabledExtensionNames
      nullptr                                         // pEnabledFeatures - must be NULL because the VkDeviceCreateInfo::pNext chain includes VkPhysicalDeviceFeatures2.
    };

    createInfo.setPNext(&deviceFeatures2);

    m_device = g_physicalDevice.createDeviceUnique(createInfo);
    g_device = m_device.get();

    if (!m_device)
      RX_ERROR("Failed to create logical device.");
  }

  void Device::checkExtensionSupport(const std::vector<const char*>& extensions) const
  {
    // Stores the name of the extension and a bool that tells if they were found or not.
    std::map<const char*, bool> requiredExtensions;

    for (const auto& extension : extensions)
      requiredExtensions.emplace(extension, false);

    std::vector<vk::ExtensionProperties> physicalDeviceExtensions = g_physicalDevice.enumerateDeviceExtensionProperties();

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