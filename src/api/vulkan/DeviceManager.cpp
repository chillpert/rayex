#include "api/vulkan/DeviceManager.hpp"

namespace RX
{
  DeviceManager::DeviceManager()
    : m_physicalDevice(VK_NULL_HANDLE) { }

  void DeviceManager::findPhysicalDevice(VkInstance instance)
  {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
      Error::runtime("Missing GPU with Vulkan support", Error::API);
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    size_t prevScore = 0;
    for (const auto& device : devices)
    {
      size_t score = evaluatePhysicalDevice(device);

      if (score > prevScore)
        m_physicalDevice = device;

      prevScore = score;
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
    {
      Error::runtime("Could not find a suitable GPU", Error::API);
    }

  #ifdef RX_DEBUG
    printPhysicalDeviceInfo();
  #endif
  }

  VkPhysicalDeviceProperties DeviceManager::getPhysicalDeviceProperties(VkPhysicalDevice device)
  {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    return deviceProperties;
  }

  VkPhysicalDeviceFeatures DeviceManager::getPhysicalDeviceFeatures(VkPhysicalDevice device)
  {
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    return deviceFeatures;
  }

  size_t DeviceManager::evaluatePhysicalDevice(VkPhysicalDevice device)
  {
    QueueFamilyIndices indices = findQueueFamilies(device);

    if (!indices.graphicsFamily.has_value())
      return 0;

    VkPhysicalDeviceProperties properties = getPhysicalDeviceProperties(device);
    VkPhysicalDeviceFeatures features = getPhysicalDeviceFeatures(device);
    
    size_t score = 0;

    switch (properties.deviceType)
    {
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        score += 1000;
    }

    return score;
  }

  QueueFamilyIndices DeviceManager::findQueueFamilies(VkPhysicalDevice device)
  {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        indices.graphicsFamily = i;

      i++;
    }

    return indices;
  }

  void DeviceManager::printPhysicalDeviceInfo()
  {
    if (m_physicalDevice != VK_NULL_HANDLE)
    {
      VkPhysicalDeviceProperties properties = getPhysicalDeviceProperties(m_physicalDevice);

      std::cout << properties.deviceName << std::endl;

      uint32_t apiVer = properties.apiVersion;

      std::cout
        << "- API:                     " << VK_VERSION_MAJOR(apiVer) << "." << VK_VERSION_MINOR(apiVer) << "." << VK_VERSION_PATCH(apiVer) << std::endl
        << "- Driver:                  " << properties.driverVersion << std::endl
        << "- Device Type:             " << properties.deviceType << std::endl
        << "- Vendor ID:               " << properties.vendorID << std::endl
        << "- Device ID:               " << properties.deviceID << std::endl
        << "- discreteQueuePrioreties: " << properties.limits.discreteQueuePriorities << std::endl << 
      std::endl;    
    }
    else
    {
      Error::runtime("Can not print physical device information, because it has not been determined yet", Error::API);
    }
  }
}