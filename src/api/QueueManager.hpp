#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class QueueManager
  {
  public:
    // This function should be called right after the physical device was enumerated and the 
    // surface was created. The surface has to be created before the physical device is picked.
    void create(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    static void retrieveAllQueueHandles(VkDevice device);

    static uint32_t getGraphicsIndex(); 
    static uint32_t getPresentIndex();

    // Returns a vector filled with the actual unique family indices.    
    static std::vector<uint32_t> getQueueFamilyIndices();

    // This function can be used at the time a physical device is picked.
    // It won't have an impact on the members of this function.
    static bool isComplete(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    
  private:
    static std::pair<std::optional<uint32_t>, std::optional<uint32_t>> findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    static bool created;
    static std::optional<uint32_t> graphicsIndex;
    static std::optional<uint32_t> presentIndex;

    static VkQueue graphicsQueue;
    static VkQueue presentQueue;
  };
}

#endif