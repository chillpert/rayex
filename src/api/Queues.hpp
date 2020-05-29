#ifndef QUEUES_HPP
#define QUEUES_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct QueuesInfo
  {
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR surface;
  };

  class Queues
  {
  public:
    // This function should be called right after the physical device was enumerated and the 
    // surface was created. The surface has to be created before the physical device is picked.
    void initialize(QueuesInfo& queuesInfo);

    inline uint32_t getGraphicsFamilyIndex() const { return m_graphicsIndex.value(); }
    inline uint32_t getPresentFamilyIndex() const { return m_presentIndex.value(); }

    inline VkQueue getGraphicsQueue() { return m_graphicsQueue; }
    inline VkQueue getPresentQueue() { return m_presentQueue; }

    inline std::vector<VkQueue> getGraphicsQueues() { return m_graphicsQueues; }
    inline std::vector<VkQueue> getPresentQueues() { return m_presentQueues; }
    inline std::vector<VkQueue> getTransferQueues() { return m_transferQueues; }

    inline QueuesInfo& getInfo() { return m_info; }

    void retrieveAllHandles(VkDevice device);

    void submit(VkSubmitInfo& submitInfo, VkFence fence);
    void present(VkPresentInfoKHR& presentInfo);

    // Returns a vector filled with the actual unique queue family indices.    
    std::vector<uint32_t> getQueueFamilyIndices();

    // This function can be used at the time a physical device is picked.
    static bool isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

  private:
    static std::vector<std::optional<uint32_t>> findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    std::optional<uint32_t> m_graphicsIndex;
    std::optional<uint32_t> m_presentIndex;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    std::vector<VkQueue> m_graphicsQueues;
    std::vector<VkQueue> m_presentQueues;
    std::vector<VkQueue> m_transferQueues;

    QueuesInfo m_info;
  };
}

#endif // QUEUES_HPP