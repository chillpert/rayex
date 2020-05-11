#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class CommandBuffer
  {
  public:
    inline VkCommandBuffer get() { return commandBuffer; }

    void create(VkDevice device, VkCommandPool commandPool)
    {
      VkCommandBufferAllocateInfo allocateInfo{ };
      allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocateInfo.commandPool = commandPool;
      allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocateInfo.commandBufferCount = 1;

      VK_ASSERT(vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer), "Failed to allocate command buffers");
    }

  private:
    VkCommandBuffer commandBuffer;
  };
}

#endif // COMMAND_BUFFER_HPP