#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "api/vulkan/SwapChain.hpp"

namespace RX
{
  class CommandBuffer
  {
  public:
    CommandBuffer(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice, SwapChain* swapChain);

    void createCommandPool();
    void createCommandBuffers();

    void destroyCommandPool();

  private:
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkPhysicalDevice* m_physicalDevice;
    VkDevice* m_logicalDevice;

    // Pointer to VulkanApi class member
    SwapChain* m_swapChain;
  };
}

#endif // COMMAND_BUFFER_HPP