#ifndef QUEUES_HPP
#define QUEUES_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct Queue;
  struct QueueFamily;
  struct QueueIndices;

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

    inline uint32_t getGraphicsFamilyIndex() const { return m_graphicsQueueIndex.value(); }
    inline uint32_t getPresentFamilyIndex() const { return m_presentQueueIndex.value(); }
    inline uint32_t getTransferFamilyIndex() const { return m_transferQueueIndex.value(); }

    inline VkQueue getGraphicsQueue() { return m_graphicsQueue; }
    inline VkQueue getPresentQueue() { return m_presentQueue; }
    inline VkQueue getTransferQueue() { return m_transferQueue; }

    inline std::vector<QueueFamily>& getQueueFamilies() { return m_uniqueQueueFamilies; }

    inline QueuesInfo& getInfo() { return m_info; }

    void retrieveAllHandles(VkDevice device);

    void print();

    void submit(VkSubmitInfo& submitInfo, VkFence fence);
    void present(VkPresentInfoKHR& presentInfo);

    std::vector<uint32_t> getQueuesWithSwapchainAccess(); // This will probably turn out to be all the graphics and present queues which are already stored in members
    std::vector<uint32_t> getQueueFamilyIndices();

    // This function can be used at the time a physical device is picked.
    static bool isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    static bool hasDedicatedTransferQueueFamily(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

  private:
    static QueueIndices findQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    static std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice physicalDevice);

    std::optional<uint32_t> m_graphicsQueueIndex;
    std::optional<uint32_t> m_presentQueueIndex;
    std::optional<uint32_t> m_transferQueueIndex;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkQueue m_transferQueue;

    std::vector<QueueFamily> m_uniqueQueueFamilies;

    std::vector<std::shared_ptr<Queue>> m_graphicsQueues;
    std::vector<std::shared_ptr<Queue>> m_presentQueues;
    std::vector<std::shared_ptr<Queue>> m_transferQueues;

    QueuesInfo m_info;
  };

  struct Queue
  {
    Queue(uint32_t i, float p) :
      queue(VK_NULL_HANDLE), index(i), priority(p) { }

    VkQueue queue;
    uint32_t index;
    float priority = 1.0f;

    friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Queue> queue);
  };

  struct QueueFamily
  {
    uint32_t index;
    std::vector<std::shared_ptr<Queue>> queues; // The queues in the given index

    friend std::ostream& operator<<(std::ostream& os, const QueueFamily& queueFamily);
  };

  struct QueueIndices // TODO: bad name
  {
    // Stores family indices and each entries queue priority. 
    std::vector<uint32_t> graphicsQueueIndices;
    std::vector<uint32_t> presentQueueIndices;
    std::vector<uint32_t> transferQueueIndices;
  };
}

#endif // QUEUES_HPP