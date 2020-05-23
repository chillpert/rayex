#include "Queues.hpp"

namespace RX
{
  Queues::Queues() :
    BaseComponent("Queues") { }

  void Queues::initialize(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    if (physicalDevice == VK_NULL_HANDLE)
      RX_ERROR("Queue families can not be set up because a physical device has not been picked yet.");

    if (surface == VK_NULL_HANDLE)
      RX_ERROR("Queue families can not be set up because the surface has not been initialized yet.");

    auto temp = findQueueFamilies(physicalDevice, surface);
    m_graphicsIndex = temp[0];
    m_presentIndex = temp[1];
    m_transferIndex = temp[2];

    RX_INITIALIZATION_CALLBACK;
  }

  void Queues::retrieveAllHandles(VkDevice device)
  {
    RX_ASSERT_INITIALIZED("retrieveAllHandles");

    vkGetDeviceQueue(device, getGraphicsIndex(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(device, getPresentIndex(), 0, &m_presentQueue);
    vkGetDeviceQueue(device, getTransferIndex(), 0, &m_transferQueue);
  }

  VkResult Queues::submit(VkSubmitInfo& submitInfo, VkFence fence)
  {
    RX_ASSERT_INITIALIZED("submit");

    VkResult res = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, fence);
    VK_ASSERT(res, "Failed to submit queue.");

    return res;
  }

  VkResult Queues::present(VkPresentInfoKHR& presentInfo)
  {
    RX_ASSERT_INITIALIZED("present");

    VkResult res = vkQueuePresentKHR(m_presentQueue, &presentInfo);
    VK_ASSERT(res, "Failed to present");

    return res;
  }

  uint32_t Queues::getGraphicsIndex() const
  {
    RX_ASSERT_INITIALIZED("getGraphicsIndex");

    return m_graphicsIndex.value();
  }

  uint32_t Queues::getPresentIndex() const
  {
    RX_ASSERT_INITIALIZED("getPresentIndex");
    
    return m_presentIndex.value();
  }

  uint32_t Queues::getTransferIndex() const
  {
    RX_ASSERT_INITIALIZED("getTransferIndex");

    return m_transferIndex.value();
  }

  std::vector<uint32_t> Queues::getQueueFamilyIndices()
  {
    RX_ASSERT_INITIALIZED("getQueueFamilyIndices");
    
    if (getPresentIndex() == getGraphicsIndex())
      return { getGraphicsIndex(), getTransferIndex() };

    return { getGraphicsIndex(), getPresentIndex(), getTransferIndex() };
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
    std::optional<uint32_t> transferIndex_t;

    uint32_t propertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertiesCount, nullptr);

    std::vector<VkQueueFamilyProperties> properties(propertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertiesCount, properties.data());

    uint32_t index = 0;
    for (const auto& property : properties)
    {
      if (property.queueFlags & VK_QUEUE_TRANSFER_BIT && !(property.queueFlags & VK_QUEUE_GRAPHICS_BIT))
        transferIndex_t = index;

      if (property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        graphicsIndex_t = index;

      // Check if the current queue index is able to present.
      VkBool32 supported;
      VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &supported), "Failed to get physical device surface support.");

      if (supported)
        presentIndex_t = index;

      if (graphicsIndex_t.has_value() && presentIndex_t.has_value() && transferIndex_t.has_value())
        break;           

      ++index;
    }

    if (!graphicsIndex_t.has_value() || !presentIndex_t.has_value() || !transferIndex_t.has_value())
      RX_ERROR("Missing queue families.");

    return { graphicsIndex_t, presentIndex_t, transferIndex_t };
  }
}