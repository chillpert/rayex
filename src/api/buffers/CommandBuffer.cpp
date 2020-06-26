#include "CommandBuffer.hpp"

namespace RX
{
  CommandBuffer::CommandBuffer(CommandBufferInfo& info)
  {
    initialize(info);
  }

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
  }

  void CommandBuffer::submitToQueue(const vk::Queue const queue) const
  {
    if (m_info.usageFlags & vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    {
      queue.submit(1, &m_info.submitInfo, nullptr); // TODO: update assert
      queue.waitIdle();
    }
    else
      RX_ERROR("Only command buffers with a usage flag containing eOneTimeSubmit should be submitted automatically");
  }
}