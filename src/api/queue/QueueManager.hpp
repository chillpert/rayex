#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

#include "Queue.hpp"

namespace RX
{
  struct QueueFamily;
  struct QueueIndices;

  struct QueuesInfo
  {
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceKHR surface;

    size_t requestGraphicsQueueAmount = SIZE_MAX; // TODO: Not implemented yet
    size_t requestPresentQueueAmount = SIZE_MAX; // TODO: Not implemented yet
    size_t requestTransferQueueAmount = SIZE_MAX; // TODO: Not implemented yet
  };

  class QueueManager
  {
  public:
    // This function should be called right after the physical device was enumerated and the 
    // surface was created. The surface has to be created before the physical device is picked.
    void initialize(QueuesInfo& queuesInfo);

    inline uint32_t getGraphicsFamilyIndex() const { return m_graphicsQueues[0]->getIndex(); }
    inline uint32_t getPresentFamilyIndex() const { return m_presentQueues[0]->getIndex(); }
    inline uint32_t getTransferFamilyIndex() const { return m_transferQueues[0]->getIndex(); }

    std::vector<uint32_t> getUniqueQueueIndices(std::vector<QueueCapability> list);

    std::shared_ptr<Queue> getQueue(QueueCapability type, int queueFamilyIndex = -1);

    inline std::vector<QueueFamily>& getQueueFamilies() { return m_queueFamilies; }

    std::vector<uint32_t> getAllFamilyIndicesOfType(QueueCapability type);

    void retrieveAllHandles(vk::Device device);

    void print();

    void submit(vk::SubmitInfo& submitInfo, vk::Fence fence, size_t index = 0); // TODO: These functions are absolutely terrible.
    void present(vk::PresentInfoKHR& presentInfo, size_t index = 0); // TODO: These functions are absolutely terrible.

    std::vector<uint32_t> getQueueFamilyIndicesForSwapchainAccess(); // This will probably turn out to be all the graphics and present queues which are already stored in members

    // This function can be used at the time a physical device is picked.
    static bool isComplete(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
    static bool hasDedicatedTransferQueueFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

  private:
    static QueueIndices findQueueFamilyIndices(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
    static std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties(vk::PhysicalDevice physicalDevice) { return physicalDevice.getQueueFamilyProperties(); }

    std::vector<QueueFamily> m_queueFamilies;

    std::vector<std::shared_ptr<Queue>> m_graphicsQueues;
    std::vector<std::shared_ptr<Queue>> m_presentQueues;
    std::vector<std::shared_ptr<Queue>> m_transferQueues;

    QueuesInfo m_info;
  };

  struct QueueFamily
  {
    uint32_t index;
    int capability;
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

#endif // QUEUE_MANAGER_HPP