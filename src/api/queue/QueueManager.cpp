#include "QueueManager.hpp"

namespace RX
{
  void QueueManager::initialize(QueuesInfo& info)
  {
    m_info = info;

    auto queueFamilyProperties = getQueueFamilyProperties(m_info.physicalDevice);
    m_queueFamilies.resize(queueFamilyProperties.size());
    
    const float queuePriority = 1.0f;

    // Create a queue wrapper for each queue available on the device.
    for (uint32_t index = 0; index < static_cast<uint32_t>(m_queueFamilies.size()); ++index)
    {
      m_queueFamilies[index].index = index;

      int capability = 0;

      if (queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        capability |= GRAPHICS;

      if (queueFamilyProperties[index].queueFlags & VK_QUEUE_TRANSFER_BIT)
        capability |= TRANSFER;

      VkBool32 supported;
      VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(m_info.physicalDevice, index, m_info.surface, &supported), "Failed to get physical device surface support.");

      if (supported)
        capability |= PRESENT;

      m_queueFamilies[index].capability = capability;

      for (uint32_t i = 0; i < queueFamilyProperties[index].queueCount; ++i)       
        m_queueFamilies[index].queues.push_back(std::make_shared<Queue>(index, capability, queuePriority));
    }

    for (uint32_t index = 0; index < static_cast<uint32_t>(m_queueFamilies.size()); ++index)
    {
      for (const std::shared_ptr<Queue> queue : m_queueFamilies[index].queues)
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
      RX_PRINT(queue);

    RX_PRINT("Present Queues");
    for (const std::shared_ptr<Queue>& queue : m_presentQueues)
      RX_PRINT(queue);

    RX_PRINT("Transfer Queues");
    for (const std::shared_ptr<Queue>& queue : m_transferQueues)
      RX_PRINT(queue);

    RX_PRINT("");

    RX_FORMAT_INFO("Queues sorted by their family index:");
    for (auto queueFamily : m_queueFamilies)
      RX_PRINT(queueFamily);
  }

  std::vector<uint32_t> QueueManager::getUniqueQueueIndices(std::vector<QueueCapability> list)
  {
    std::unordered_set<uint32_t> temp;
    
    auto graphicsFamilyIndices = getAllFamilyIndicesOfType(GRAPHICS);
    auto presentFamilyIndices = getAllFamilyIndicesOfType(PRESENT);
    auto transferFamilyIndices = getAllFamilyIndicesOfType(TRANSFER);

    size_t requiredGraphicsIndices = 0;
    size_t requiredPresentIndices = 0;
    size_t requiredTransferIndices = 0;

    for (QueueCapability capbility : list)
    {
      if (capbility == GRAPHICS)
        ++requiredGraphicsIndices;

      if (capbility == PRESENT)
        ++requiredPresentIndices;

      if (capbility == TRANSFER)
        ++requiredTransferIndices;
    }

    for (QueueCapability capbility : list)
    {
      if (capbility == GRAPHICS && requiredGraphicsIndices > 0)
      {
        for (uint32_t index : graphicsFamilyIndices)
        {
          auto result = temp.insert(index);
          if (result.second)
          {
            --requiredGraphicsIndices;
            break;
          }
        }
      }

      if (capbility == PRESENT && requiredPresentIndices > 0)
      {
        for (uint32_t index : presentFamilyIndices)
        {
          auto result = temp.insert(index);
          if (result.second)
          {
            --requiredPresentIndices;
            break;
          }
        }
      }

      if (capbility == TRANSFER && requiredTransferIndices > 0)
      {
        for (uint32_t index : transferFamilyIndices)
        {
          auto result = temp.insert(index);
          if (result.second)
          {
            --requiredTransferIndices;
            break;
          }
        }
      }
    }

    std::vector<uint32_t> res;
    std::copy(temp.cbegin(), temp.cend(), std::back_inserter(res));
    
    if (list.size() != res.size())
    {
      RX_PRINT("Failed to retrieve unique queue family indices for the specified configuration. Returning empty vector ...");
      return std::vector<uint32_t>();
    }

    return res;
  }

  VkQueue QueueManager::getGraphicsQueue(int queueFamilyIndex)
  {
    if (queueFamilyIndex == -1)
      return m_graphicsQueues[0]->getQueue();

    for (const std::shared_ptr<Queue>& queue : m_graphicsQueues)
    {
      if (queue->getIndex() == queueFamilyIndex)
        return queue->getQueue();
    }

    RX_LOG("There is no graphics queue available on the specified queue family index. Returning any graphics queue instead ...");
    return m_graphicsQueues[0]->getQueue();
  }

  VkQueue QueueManager::getPresentQueue(int queueFamilyIndex)
  {
    if (queueFamilyIndex == -1)
      return m_presentQueues[0]->getQueue();

    for (const std::shared_ptr<Queue>& queue : m_presentQueues)
    {
      if (queue->getIndex() == queueFamilyIndex)
        return queue->getQueue();
    }

    RX_LOG("There is no present queue available on the specified queue family index. Returning any present queue instead ...");
    return m_presentQueues[0]->getQueue();
  }

  VkQueue QueueManager::getTransferQueue(int queueFamilyIndex)
  {
    if (queueFamilyIndex == -1)
      return m_transferQueues[0]->getQueue();

    for (const std::shared_ptr<Queue>& queue : m_transferQueues)
    {
      if (queue->getIndex() == queueFamilyIndex)
        return queue->getQueue();
    }

    RX_LOG("There is no transfer queue available on the specified queue family index. Returning any transfer queue instead ...");
    return m_transferQueues[0]->getQueue();
  }

  std::vector<uint32_t> QueueManager::getAllFamilyIndicesOfType(QueueCapability type)
  {
    std::vector<uint32_t> result;

    for (QueueFamily& family : m_queueFamilies)
    {
      if ((family.capability & type) == type)
        result.push_back(family.index);
    }

    return result;
  }

  void QueueManager::retrieveAllHandles(VkDevice device)
  {
    for (std::shared_ptr<Queue> queue : m_graphicsQueues)
      vkGetDeviceQueue(device, queue->getIndex(), 0, &queue->getQueue());

    for (std::shared_ptr<Queue> queue : m_presentQueues)
      vkGetDeviceQueue(device, queue->getIndex(), 0, &queue->getQueue());
      
    for (std::shared_ptr<Queue> queue : m_transferQueues)
      vkGetDeviceQueue(device, queue->getIndex(), 0, &queue->getQueue());
  }

  void QueueManager::print()
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
          RX_PRINT_CONTINOUS(properties[i] << " | ");
        else
          RX_PRINT(properties[i]);
      }
      RX_PRINT("");
    }
  }

  void QueueManager::submit(VkSubmitInfo& submitInfo, VkFence fence, size_t index)
  {
    VK_ASSERT(vkQueueSubmit(m_graphicsQueues[index]->getQueue(), 1, &submitInfo, fence), "Failed to submit queue.");
  }

  void QueueManager::present(VkPresentInfoKHR& presentInfo, size_t index)
  {
    VK_ASSERT(vkQueuePresentKHR(m_presentQueues[index]->getQueue(), &presentInfo), "Failed to present");
  }

  std::vector<uint32_t> QueueManager::getQueueFamilyIndicesForSwapchainAccess()
  {
    if (getPresentFamilyIndex() == getGraphicsFamilyIndex())
      return { getGraphicsFamilyIndex() };

    return { getGraphicsFamilyIndex(), getPresentFamilyIndex() };
  }

  bool QueueManager::isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
  {
    auto temp = findQueueFamilyIndices(physicalDevice, surface);

    if (temp.graphicsQueueIndices.size() == 0 || temp.presentQueueIndices.size() == 0 || temp.transferQueueIndices.size() == 0)
      return false;

    return true;
  }

  bool QueueManager::hasDedicatedTransferQueueFamily(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
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

  QueueIndices QueueManager::findQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
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
  
  std::vector<VkQueueFamilyProperties> QueueManager::getQueueFamilyProperties(VkPhysicalDevice physicalDevice)
  {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    RX_ASSERT((queueFamilyCount > 0), "Failed to retrieve any queue family index");

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    RX_ASSERT((queueFamilies.size() > 0), "Failed to retrieve queue family properties");

    return queueFamilies;
  }

  std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Queue> queue)
  {
    os << "\tIndex: " << queue->getIndex() << " | Priority: " << queue->getPriority();
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