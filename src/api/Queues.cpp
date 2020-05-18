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
    m_graphicsIndex = temp.first;
    m_presentIndex = temp.second; 

    initializationCallback();
  }

  void Queues::retrieveAllHandles(VkDevice device)
  {
    assertInitialized("retrieveAllHandles");

    vkGetDeviceQueue(device, getGraphicsIndex(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(device, getPresentIndex(), 0, &m_presentQueue);
  }

  void Queues::submit(VkSubmitInfo& submitInfo, VkFence fence)
  {
    assertInitialized("submit");

    VK_ASSERT(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, fence), "Failed to submit queue.");
  }

  void Queues::present(VkPresentInfoKHR& presentInfo)
  {
    assertInitialized("present");

    VK_ASSERT(vkQueuePresentKHR(m_presentQueue, &presentInfo), "Failed to present");
  }

  uint32_t Queues::getGraphicsIndex()
  {
    assertInitialized("getGraphicsIndex");

    return m_graphicsIndex.value();
  }

  uint32_t Queues::getPresentIndex()
  {
    assertInitialized("getPresentIndex");
    
    return m_presentIndex.value();
  }

  std::vector<uint32_t> Queues::getQueueFamilyIndices()
  {
    assertInitialized("getQueueFamilyIndices");
    
    if (getPresentIndex() == getGraphicsIndex())
      return { getGraphicsIndex() };

    return { getGraphicsIndex(), getPresentIndex() };
  }

  bool Queues::isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    auto temp = findQueueFamilies(physicalDevice, surface);
    
    if (temp.first.has_value() && temp.second.has_value())
      return true;

    return false;
  }

  std::pair<std::optional<uint32_t>, std::optional<uint32_t>> Queues::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
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

    return { graphicsIndex_t, presentIndex_t };
  }
}