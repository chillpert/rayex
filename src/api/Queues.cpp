#include "Queues.hpp"

namespace RX
{
  void Queues::initialize(QueuesInfo& info)
  {
    m_info = info;

    std::vector<std::optional<uint32_t>> temp = findQueueFamilies(info.physicalDevice, info.surface);
    m_graphicsIndex = temp[0];
    m_presentIndex = temp[1];
  }

  void Queues::retrieveAllHandles(VkDevice device)
  {
    vkGetDeviceQueue(device, getGraphicsFamilyIndex(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(device, getPresentFamilyIndex(), 0, &m_presentQueue);
  }

  void Queues::submit(VkSubmitInfo& submitInfo, VkFence fence)
  {
    VK_ASSERT(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, fence), "Failed to submit queue.");
  }

  void Queues::present(VkPresentInfoKHR& presentInfo)
  {
    VK_ASSERT(vkQueuePresentKHR(m_presentQueue, &presentInfo), "Failed to present");
  }

  std::vector<uint32_t> Queues::getQueueFamilyIndices()
  {
    if (getPresentFamilyIndex() == getGraphicsFamilyIndex())
      return { getGraphicsFamilyIndex() };

    return { getGraphicsFamilyIndex(), getPresentFamilyIndex() };
  }

  bool Queues::isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    auto queueFamiliesIndices = findQueueFamilies(physicalDevice, surface);

    for (auto& index : queueFamiliesIndices)
    {
      if (!index.has_value())
        return false;
    }
  
    return true;
  }

  std::vector<std::optional<uint32_t>> Queues::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    std::optional<uint32_t> graphicsIndex_t;
    std::optional<uint32_t> presentIndex_t;

    uint32_t propertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertiesCount, nullptr);

    std::vector<VkQueueFamilyProperties> properties(propertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertiesCount, properties.data());

    uint32_t index = 0;
    for (const auto& property : properties)
    {
      if (property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        graphicsIndex_t = index;

      // Check if the current queue index is able to present.
      VkBool32 supported;
      VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &supported), "Failed to get physical device surface support.");

      if (supported)
        presentIndex_t = index;

      if (graphicsIndex_t.has_value() && presentIndex_t.has_value())
        break;           

      ++index;
    }

    if (!graphicsIndex_t.has_value() || !presentIndex_t.has_value())
      RX_ERROR("Missing queue families.");

    return { graphicsIndex_t, presentIndex_t };
  }
}