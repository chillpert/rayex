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

    std::vector<std::pair<unsigned int, std::string>> results;

    unsigned int score = 0;
    for (const auto& it : physicalDevices)
    {
      auto temp = evaluate(it);
      results.push_back(temp);

      if (temp.first > score)
      {
        m_physicalDevice = it;
        score = temp.first;
      }
    }

    if (score == 0 || m_physicalDevice == VK_NULL_HANDLE)
      RX_ERROR("No suitable device was found");

    RX_FORMAT_TABLE2(
      "Physical device report: ",
      "Device name",
      "Score",
      for (const auto& result : results)
        RX_FORMAT_TABLE2_ENTRY(result.second, result.first);
      RX_EMPTY_LINE;
    );

    // Print information about the GPU that was selected.
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &props);
    RX_LOG("Selected GPU: " << props.deviceName);
  }

  std::pair<unsigned int, std::string> PhysicalDevice::evaluate(VkPhysicalDevice device) const
  {
    unsigned int score = 0u;

    // Check the device's features and properties.
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(device, &props);

    std::string deviceName = props.deviceName;

    VkPhysicalDeviceFeatures feats;
    vkGetPhysicalDeviceFeatures(device, &feats);

    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      score += 100u;
    else
      return { 0u, deviceName };

#ifdef VK_API_VERSION_1_1
    if (props.apiVersion >= VK_API_VERSION_1_1)
      score += 10u;
#endif

#ifdef VK_API_VERSION_1_2
    if (props.apiVersion >= VK_API_VERSION_1_2)
      score += 10u;
#endif

    if (deviceName.find("RTX") != std::string::npos)
      score += 100u;

    if (Queues::isComplete(device, m_info.surface))
      score += 100u;
    else
      return { 0u, deviceName };

    // TODO: add more hardware specific evaulation (those that are benefitial for path tracing)
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features

    return { score, deviceName };
  }
}