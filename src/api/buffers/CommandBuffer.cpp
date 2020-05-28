#include "CommandBuffer.hpp"

namespace RX
{
  void CommandBuffer::initialize(CommandBufferInfo& info)
  {
    m_info = info;

    m_commandBuffers.resize(m_info.commandBufferCount);

    VkCommandBufferAllocateInfo allocateInfo{ };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = m_info.commandPool;
    allocateInfo.level = m_info.level;
    allocateInfo.commandBufferCount = static_cast<uint32_t>(m_info.commandBufferCount);

    VK_CREATE(vkAllocateCommandBuffers(m_info.device, &allocateInfo, m_commandBuffers.data()), "command buffer(s)");

    // set up begin info.
    m_info.beginInfo.flags = m_info.usageFlags;

    // Set up submit info.
    m_info.submitInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
    m_info.submitInfo.pCommandBuffers = m_commandBuffers.data();
  }

  void CommandBuffer::free()
  {
    VK_FREE(vkFreeCommandBuffers(m_info.device, m_info.commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data()), "command buffer(s)");
  }

  void CommandBuffer::begin(size_t index)
  {
    VK_ASSERT(vkBeginCommandBuffer(m_commandBuffers[index], &m_info.beginInfo), "Failed to begin command buffer(s)");
  }

  void CommandBuffer::end(size_t index)
  {
    VK_ASSERT(vkEndCommandBuffer(m_commandBuffers[index]), "Failed to end command buffer(s)");

    if (m_info.usageFlags & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
    {
      RX_ASSERT(m_info.queue != VK_NULL_HANDLE, "Command buffer object is one time usage, but no queue for immediate execution was specified.");

      VK_ASSERT(vkQueueSubmit(m_info.queue, 1, &m_info.submitInfo, VK_NULL_HANDLE), "failed to submit queue of one time usage command buffer");
      VK_ASSERT(vkQueueWaitIdle(m_info.queue), "Queue for one time usage command buffer failed to wait idle");

      free();
    }
  }

  void CommandBuffer::record()
  {
    
  }
}