#include "Queues.hpp"

namespace RX
{
  void Queues::initialize(QueuesInfo& info)
  {
    m_info = info;
    auto queueFamilyIndices = findQueueFamilyIndices(info.physicalDevice, info.surface);
    
    // NOTE: Legacy
    m_graphicsQueueIndex = queueFamilyIndices.graphicsQueueIndices[0];
    m_presentQueueIndex = queueFamilyIndices.presentQueueIndices[0];
    m_transferQueueIndex = queueFamilyIndices.transferQueueIndices[0];

    auto queueFamilyProperties = getQueueFamilyProperties(m_info.physicalDevice);
    m_uniqueQueueFamilies.resize(queueFamilyProperties.size());
    
    const float queuePriority = 1.0f;

    // Create a queue wrapper for each queue available on the device.
    for (uint32_t index = 0; index < m_uniqueQueueFamilies.size(); ++index)
    {
      for (uint32_t i = 0; i < queueFamilyProperties[index].queueCount; ++i)
        m_uniqueQueueFamilies[index].queues.push_back(std::make_shared<Queue>(index, queuePriority));
    }

    for (uint32_t index = 0; index < m_uniqueQueueFamilies.size(); ++index)
    {
      for (const std::shared_ptr<Queue> queue : m_uniqueQueueFamilies[index].queues)
      {
        if (queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
          m_graphicsQueues.push_back(queue);

        VkBool32 supported;
        VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(m_info.physicalDevice, index, m_info.surface, &supported), "Failed to get physical device surface support.");

        if (supported)
          m_presentQueues.push_back(queue);

        if (queueFamilyProperties[index].queueFlags & VK_QUEUE_TRANSFER_BIT)
          m_transferQueues.push_back(queue);
      }
    }

    RX_FORMAT_INFO("Queues sorted by their capabilities:");
    RX_PRINT("Graphics Queues");
    for (const std::shared_ptr<Queue>& queue : m_graphicsQueues)
      std::cout << queue << std::endl;

    RX_PRINT("Present Queues");
    for (const std::shared_ptr<Queue>& queue : m_presentQueues)
      std::cout << queue << std::endl;

    RX_PRINT("Transfer Queues");
    for (const std::shared_ptr<Queue>& queue : m_transferQueues)
      std::cout << queue << std::endl;

    RX_PRINT("");

    RX_FORMAT_INFO("Queues sorted by their family index:");
    for (auto queueFamily : m_uniqueQueueFamilies)
      RX_PRINT(queueFamily);
  }

  void Queues::retrieveAllHandles(VkDevice device)
  {
    // TODO: iterate over all graphics / present / transfer queues and retrieve each handle
    vkGetDeviceQueue(device, getGraphicsFamilyIndex(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(device, getPresentFamilyIndex(), 0, &m_presentQueue);
    // TODO: retrieve transfer queue
  }

  void Queues::print()
  {
    std::vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilyProperties(m_info.physicalDevice);
    uint32_t queueFamilyIndicesCount = static_cast<uint32_t>(queueFamilies.size());

    RX_FORMAT_INFO("Queue Report:");
    for (uint32_t index = 0; index < queueFamilyIndicesCount; ++index)
    {
      std::vector<std::string> properties;

      RX_PRINT("Queue Family Index: " << index << " | " << queueFamilies[index].queueCount << " Queue(s)");
      if (queueFamilies[index].queueFlags & VK_QUEUE_TRANSFER_BIT)
        properties.push_back(std::string("Transfer"));

      if (queueFamilies[index].queueFlags & VK_QUEUE_COMPUTE_BIT)
        properties.push_back(std::string("Compute"));

      if (queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        properties.push_back(std::string("Graphics"));
      }

      VkBool32 supported;
      VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(m_info.physicalDevice, index, m_info.surface, &supported), "Failed to get physical device surface support.");
      
      if (supported)
        properties.push_back(std::string("Present"));

      if (queueFamilies[index].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        properties.push_back(std::string("Sparse"));

      if (queueFamilies[index].queueFlags & VK_QUEUE_PROTECTED_BIT)
        properties.push_back(std::string("Protected"));

      for (size_t i = 0; i < properties.size(); ++i)
      {
        if (i + 1 < properties.size())
          std::cout << properties[i] << " | ";
        else
          std::cout << properties[i] << std::endl;
      }
      RX_PRINT("");
    }
  }

  void Queues::submit(VkSubmitInfo& submitInfo, VkFence fence)
  {
    VK_ASSERT(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, fence), "Failed to submit queue.");
  }

  void Queues::present(VkPresentInfoKHR& presentInfo)
  {
    VK_ASSERT(vkQueuePresentKHR(m_presentQueue, &presentInfo), "Failed to present");
  }

  std::vector<uint32_t> Queues::getQueuesWithSwapchainAccess()
  {
    if (getPresentFamilyIndex() == getGraphicsFamilyIndex())
      return { getGraphicsFamilyIndex() };

    return { getGraphicsFamilyIndex(), getPresentFamilyIndex() };
  }

  std::vector<uint32_t> Queues::getQueueFamilyIndices()
  {
    if (getPresentFamilyIndex() == getGraphicsFamilyIndex())
      return { getGraphicsFamilyIndex() };

    return { getGraphicsFamilyIndex(), getPresentFamilyIndex() };
  }

  bool Queues::isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    auto temp = findQueueFamilyIndices(physicalDevice, surface);

    if (temp.graphicsQueueIndices.size() == 0 || temp.presentQueueIndices.size() == 0 || temp.transferQueueIndices.size() == 0)
      return false;

    return true;
  }

  bool Queues::hasDedicatedTransferQueueFamily(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    auto temp = findQueueFamilyIndices(physicalDevice, surface);

    std::vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilyProperties(physicalDevice);
    uint32_t queueFamilyIndicesCount = static_cast<uint32_t>(queueFamilies.size());

    for (uint32_t index = 0; index < temp.transferQueueIndices.size(); ++index)
    {
      if (!(queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT))
        return true;
    }

    return false;
  }

  QueueIndices Queues::findQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    std::vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilyProperties(physicalDevice);
    uint32_t queueFamilyIndicesCount = static_cast<uint32_t>(queueFamilies.size());

    // Get all possible queue family indices with transfer support.
    std::vector<uint32_t> graphicsQueueFamilyIndices;
    std::vector<uint32_t> presentQueueFamilyIndices;
    std::vector<uint32_t> transferQueueFamilyIndices;

    const float queuePriority = 1.0f;

    for (uint32_t index = 0; index < queueFamilyIndicesCount; ++index)
    {
      // Make sure the current queue family index contains at least one queue.
      if (queueFamilies[index].queueCount == 0)
        continue;

      if (queueFamilies[index].queueFlags & VK_QUEUE_TRANSFER_BIT)
        transferQueueFamilyIndices.push_back(index);

      if (queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        graphicsQueueFamilyIndices.push_back(index);

      VkBool32 supported;
      VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &supported), "Failed to get physical device surface support.");

      if (supported)
        presentQueueFamilyIndices.push_back(index);
    }

    return { graphicsQueueFamilyIndices, presentQueueFamilyIndices, transferQueueFamilyIndices };
  }
  
  std::vector<VkQueueFamilyProperties> Queues::getQueueFamilyProperties(VkPhysicalDevice physicalDevice)
  {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    RX_ASSERT(queueFamilyCount > 0, "Failed to retrieve any queue family index");

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    RX_ASSERT(queueFamilies.size() > 0, "Failed to retrieve queue family properties");

    return queueFamilies;
  }

  std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Queue> queue)
  {
    os << "\tIndex: " << queue->index << " | Priority: " << queue->priority;
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const QueueFamily& queueFamily)
  {
    os << "Queue Family Index: " << queueFamily.index << std::endl;

    for (const std::shared_ptr<Queue> queue : queueFamily.queues)
      os << queue << std::endl;

    os << std::endl;

    return os;
  }
}