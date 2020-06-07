#include "CommandBuffer.hpp"

namespace RX
{
  void CommandBuffer::initialize(CommandBufferInfo& info)
  {
    m_info = info;

    m_commandBuffers.resize(m_info.commandBufferCount);

    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = m_info.commandPool;
    allocateInfo.level = m_info.level;
    allocateInfo.commandBufferCount = static_cast<uint32_t>(m_info.commandBufferCount);

    m_commandBuffers = m_info.device.allocateCommandBuffers(allocateInfo);

    // set up begin info.
    m_info.beginInfo.flags = m_info.usageFlags;

    // Set up submit info.
    m_info.submitInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
    m_info.submitInfo.pCommandBuffers = m_commandBuffers.data();
  }

  void CommandBuffer::free()
  {
    m_info.device.freeCommandBuffers(m_info.commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
  }

  void CommandBuffer::reset()
  {
    for (vk::CommandBuffer& buffer : m_commandBuffers)
      buffer.reset(m_info.resetFlags);
  }

  void CommandBuffer::begin(size_t index)
  {
    m_commandBuffers[index].begin(m_info.beginInfo);
  }

  void CommandBuffer::end(size_t index)
  {
    m_commandBuffers[index].end();

    if (m_info.usageFlags & vk::CommandBufferUsageFlagBits::eOneTimeSubmit && m_info.submitAutomatically)
    {
      m_info.queue.submit(1, &m_info.submitInfo, nullptr);
      m_info.queue.waitIdle();
    }
  }
}