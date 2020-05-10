#include "api/Device.hpp"
#include "api/SwapChain.hpp"
#include "utils/VkUtils.hpp"

namespace RX
{
  const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  Device::Device(VkInstance* instance) :
    m_physicalDevice(VK_NULL_HANDLE),
    m_logicalDevice(VK_NULL_HANDLE),
    m_graphicsQueue(VK_NULL_HANDLE),
    m_presentQueue(VK_NULL_HANDLE),
    m_instance(instance) { }

  void Device::enumeratePhysicalDevice()
  {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*m_instance, &deviceCount, nullptr);
    
    if (deviceCount == 0)
    {
      Error::runtime("Missing GPU with Vulkan support", Error::API);
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*m_instance, &deviceCount, devices.data());

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

  void Device::createLogicalDevice()
  {
    if (m_physicalDevice == VK_NULL_HANDLE)
    {
      Error::runtime("Can not create logical device before creating physical device", Error::API);
    }

    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
      VkDeviceQueueCreateInfo queueCreateInfo { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
    }

    // physical device features
    VkPhysicalDeviceFeatures deviceFeatures{ };

    // device create info
    VkDeviceCreateInfo createInfo { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    Assert::vulkan(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice), "Failed to create logical device");

    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentQueue);
  }

  void Device::destroyLogicalDevice()
  {
    if (m_logicalDevice == VK_NULL_HANDLE)
    {
      Error::runtime("Failed to destroy logical device", Error::API);
    }

    vkDestroyDevice(m_logicalDevice, nullptr);
  }
  
  QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device)
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

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *SwapChain::getSurface(), &presentSupport);

      if (presentSupport)
        indices.presentFamily = i;

      if (indices.isComplete())
        break;

      i++;
    }

    return indices;
  }

  VkPhysicalDeviceProperties Device::getPhysicalDeviceProperties(VkPhysicalDevice device)
  {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    return deviceProperties;
  }

  VkPhysicalDeviceFeatures Device::getPhysicalDeviceFeatures(VkPhysicalDevice device)
  {
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    return deviceFeatures;
  }

  size_t Device::evaluatePhysicalDevice(VkPhysicalDevice device)
  {
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
      SwapChainSupportDetails swapChainSupport = SwapChain::querySwapChainSupport(device);
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if (!swapChainAdequate)
      return 0;

    if (!indices.isComplete())
      return 0;

    if (!checkDeviceExtensionSupport(device))
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

  bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
  {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }


  void Device::printPhysicalDeviceInfo()
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