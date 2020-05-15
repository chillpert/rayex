#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class CommandBuffer
  {
  public:
    inline VkCommandBuffer get() { return m_commandBuffer; }

    void initialize(VkDevice device, VkCommandPool commandPool)
    {
      VkCommandBufferAllocateInfo allocateInfo{ };
      allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocateInfo.commandPool = commandPool;
      allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocateInfo.commandBufferCount = 1;

      VK_ASSERT(vkAllocateCommandBuffers(device, &allocateInfo, &m_commandBuffer), "Failed to allocate command buffers");
    }

  private:
    VkCommandBuffer m_commandBuffer;
  };
}

#endif // COMMAND_BUFFER_HPP