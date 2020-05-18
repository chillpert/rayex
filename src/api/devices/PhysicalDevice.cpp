#include "PhysicalDevice.hpp"
#include "api/Queues.hpp"

namespace RX
{
  void PhysicalDevice::initialize(VkInstance instance, VkSurfaceKHR surface)
  {
    uint32_t physicalDeviceCount = 0;
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr), "Failed to enumerate physical devices");

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()), "Failed to enumerate physical devices");

    unsigned int score = 0;
    for (const auto& it : physicalDevices)
    {
      {
        unsigned int temp = evaluate(it, surface);
        if (temp > score)
        {
          m_physicalDevice = it;
          score = temp;
        }
      }
    }

    if (score == 0 || m_physicalDevice == VK_NULL_HANDLE)
      RX_ERROR("No suitable device was found");

    // Print information about the GPU that was selected.
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &props);
    VK_LOG("Selected GPU: " << props.deviceName);
  }

  void PhysicalDevice::checkExtensionSupport(const std::vector<const char*>& extensions) const
  {
    // Stores the name of the extension and a bool that tells if they were found or not.
    std::map<const char*, bool> requiredExtensions;

    for (const auto& extension : extensions)
      requiredExtensions.emplace(extension, false);

    uint32_t physicalDeviceExtensionCount;
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(m_physicalDevice, NULL, &physicalDeviceExtensionCount, nullptr), "Failed to enumerate physical device extensions");

    std::vector<VkExtensionProperties> physicalDeviceExtensions(physicalDeviceExtensionCount);
    VK_ASSERT(vkEnumerateDeviceExtensionProperties(m_physicalDevice, NULL, &physicalDeviceExtensionCount, physicalDeviceExtensions.data()), "Failed to enumerate physical device extensions");
      
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
        RX_ERROR("Missing physical device extension: " + std::string(requiredphysicalDeviceExtension.first) + ". Have you tried installing the NVIDIA Beta drivers?");
    }
  }

  unsigned int PhysicalDevice::evaluate(VkPhysicalDevice device, VkSurfaceKHR surface)
  {
    unsigned int score = 0u;

    // Check the device's features and properties.
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(device, &props);

    VkPhysicalDeviceFeatures feats;
    vkGetPhysicalDeviceFeatures(device, &feats);

    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      score += 100u;
    else
      return 0u;

#ifdef VK_API_VERSION_1_1
    if (props.apiVersion >= VK_API_VERSION_1_1)
      score += 10u;
#endif

#ifdef VK_API_VERSION_1_2
    if (props.apiVersion >= VK_API_VERSION_1_2)
      score += 10u;
#endif

    std::string name = props.deviceName;
    if (name.find("RTX") != std::string::npos)
      score += 100u;

    if (Queues::isComplete(device, surface))
      score += 100u;
    else
      return 0u;

    // TODO: add more hardware specific evaulation (those that are benefitial for path tracing)
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features

    return score;
  }
}