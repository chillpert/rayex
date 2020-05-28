#include "PhysicalDevice.hpp"
#include "api/Queues.hpp"

namespace RX
{
  void PhysicalDevice::initialize(PhysicalDeviceInfo& info)
  {
    m_info = info;

    uint32_t physicalDeviceCount = 0;
    VK_ASSERT(vkEnumeratePhysicalDevices(m_info.instance, &physicalDeviceCount, nullptr), "Failed to enumerate physical devices");

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_ASSERT(vkEnumeratePhysicalDevices(m_info.instance, &physicalDeviceCount, physicalDevices.data()), "Failed to enumerate physical devices");

    unsigned int score = 0;
    for (const auto& it : physicalDevices)
    {
      {
        unsigned int temp = evaluate(it);
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

  unsigned int PhysicalDevice::evaluate(VkPhysicalDevice device) const
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

    if (Queues::isComplete(device, m_info.surface))
      score += 100u;
    else
      return 0u;

    // TODO: add more hardware specific evaulation (those that are benefitial for path tracing)
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features

    return score;
  }
}