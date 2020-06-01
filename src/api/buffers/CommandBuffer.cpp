#include "CommandBuffer.hpp"

namespace RX
{
  CommandBuffer::~CommandBuffer()
  {
    if (m_info.freeAutomatically)
      free();
  }

  void CommandBuffer::initialize(CommandBufferInfo& info)
  {
    m_info = info;

    m_commandBuffers.resize(m_info.commandBufferCount);

    VkCommandBufferAllocateInfo allocateInfo{ };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = m_info.commandPool;
    allocateInfo.level = m_info.level;
    allocateInfo.commandBufferCount = static_cast<uint32_t>(m_info.commandBufferCount);

    VK_ALLOCATE(vkAllocateCommandBuffers(m_info.device, &allocateInfo, m_commandBuffers.data()), m_info.componentName);

    // set up begin info.
    m_info.beginInfo.flags = m_info.usageFlags;

    // Set up submit info.
    m_info.submitInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
    m_info.submitInfo.pCommandBuffers = m_commandBuffers.data();
  }

  void CommandBuffer::free()
  {
    VK_FREE(vkFreeCommandBuffers(m_info.device, m_info.commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data()), m_info.componentName);
  }

  void CommandBuffer::reset()
  {
    for (VkCommandBuffer& buffer : m_commandBuffers)
      VK_ASSERT(vkResetCommandBuffer(buffer, m_info.resetFlags), "Failed to reset " + m_info.componentName);
  }

  void CommandBuffer::begin(size_t index)
  {
    VK_ASSERT(vkBeginCommandBuffer(m_commandBuffers[index], &m_info.beginInfo), "Failed to begin " + m_info.componentName);
  }

  void CommandBuffer::end(size_t index)
  {
    VK_ASSERT(vkEndCommandBuffer(m_commandBuffers[index]), "Failed to end " + m_info.componentName);

    if (m_info.usageFlags & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
    {
      RX_ASSERT((m_info.queue != VK_NULL_HANDLE), "Command buffer object is one time usage, but no queue for immediate execution was specified.");

      VK_ASSERT(vkQueueSubmit(m_info.queue, 1, &m_info.submitInfo, VK_NULL_HANDLE), "failed to submit queue of one time usage command buffer");
      VK_ASSERT(vkQueueWaitIdle(m_info.queue), "Queue for one time usage command buffer failed to wait idle");
    }
  }
}