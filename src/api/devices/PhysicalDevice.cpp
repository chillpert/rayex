#include "PhysicalDevice.hpp"
#include "QueueManager.hpp"

namespace RX
{
  void PhysicalDevice::initialize(PhysicalDeviceInfo& info)
  {
    m_info = info;

    auto physicalDevices = m_info.instance.enumeratePhysicalDevices();
    
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

    if (!m_physicalDevice)
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
    m_properties = m_physicalDevice.getProperties();
    m_properties2 = m_physicalDevice.getProperties2();
    
    m_features = m_physicalDevice.getFeatures();
    m_features2 = m_physicalDevice.getFeatures2();

    RX_LOG("Selected GPU: " << m_properties.deviceName);
  }

  std::pair<unsigned int, std::string> PhysicalDevice::evaluate(vk::PhysicalDevice physicalDevice) const
  {
    unsigned int score = 0u;

    // Check the device's features and properties.
    auto props = physicalDevice.getProperties();
    auto feats = physicalDevice.getFeatures();

    std::string deviceName = props.deviceName;

    if (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
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

    if (QueueManager::isComplete(physicalDevice, m_info.surface))
      score += 100u;
    else
      return { 0u, deviceName };

    if (QueueManager::hasDedicatedTransferQueueFamily(physicalDevice, m_info.surface))
      score += 25;

    // TODO: add more hardware specific evaulation (those that are benefitial for path tracing)
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features

    return { score, deviceName };
  }
}