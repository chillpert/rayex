#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class QueueManager : public BaseComponent
  {
  public:
    QueueManager();

    // This function should be called right after the physical device was enumerated and the 
    // surface was created. The surface has to be created before the physical device is picked.
    void initialize(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    void retrieveAllQueueHandles(VkDevice device);
    void submit(VkSubmitInfo& submitInfo);
    void present(VkPresentInfoKHR& presentInfo);

    uint32_t getGraphicsIndex(); 
    uint32_t getPresentIndex();

    // Returns a vector filled with the actual unique family indices.    
    std::vector<uint32_t> getQueueFamilyIndices();

    // This function can be used at the time a physical device is picked.
    // It won't have an impact on the members of this function.
    static bool isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

  private:
    static std::pair<std::optional<uint32_t>, std::optional<uint32_t>> findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    std::optional<uint32_t> m_graphicsIndex;
    std::optional<uint32_t> m_presentIndex;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
  };
}

#endif