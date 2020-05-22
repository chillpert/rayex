#ifndef QUEUES_HPP
#define QUEUES_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class Queues : public BaseComponent
  {
  public:
    Queues();

    // This function should be called right after the physical device was enumerated and the 
    // surface was created. The surface has to be created before the physical device is picked.
    void initialize(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    void retrieveAllHandles(VkDevice device);
    VkResult submit(VkSubmitInfo& submitInfo, VkFence fence);
    VkResult present(VkPresentInfoKHR& presentInfo);

    uint32_t getGraphicsIndex() const;
    uint32_t getPresentIndex() const;
    uint32_t getTransferIndex() const;

    inline VkQueue getGraphicsQueue() { return m_graphicsQueue; }
    inline VkQueue getPresentQueue() { return m_presentQueue; }
    inline VkQueue getTransferQueue() { return m_transferQueue; }

    // Returns a vector filled with the actual unique family indices.    
    std::vector<uint32_t> getQueueFamilyIndices();

    // This function can be used at the time a physical device is picked.
    // It won't have an impact on the members of this function.
    static bool isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

  private:
    static std::vector<std::optional<uint32_t>> findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    std::optional<uint32_t> m_graphicsIndex;
    std::optional<uint32_t> m_presentIndex;
    std::optional<uint32_t> m_transferIndex;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkQueue m_transferQueue;
  };
}

#endif // QUEUES_HPP