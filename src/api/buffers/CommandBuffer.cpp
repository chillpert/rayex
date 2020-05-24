#include "CommandBuffer.hpp"

namespace RX
{
  void CommandBuffer::begin(VkDevice device, VkCommandPool commandPool, VkQueue queue)
  {
    m_device = device;
    m_commandPool = commandPool;
    m_queue = queue;

    VkCommandBufferAllocateInfo allocateInfo{ };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = commandPool;
    allocateInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(device, &allocateInfo, &m_commandBuffer);

    VkCommandBufferBeginInfo beginInfo{ };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
  }

  void CommandBuffer::end()
  {
    vkEndCommandBuffer(m_commandBuffer);

    VkSubmitInfo submitInfo{ };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;

    vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_queue);

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_commandBuffer);
  }
}