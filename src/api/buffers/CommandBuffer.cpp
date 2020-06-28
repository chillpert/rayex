#include "CommandBuffer.hpp"

namespace RX
{
  CmdBuffer::CmdBuffer(CmdBufferInfo& info)
  {
    init(info);
  }

  CmdBuffer::CmdBuffer(CmdBufferInfo&& info)
  {
    init(info);
  }

  void CmdBuffer::init(CmdBufferInfo& info)
  {
    m_info = info;

    m_commandBuffers.resize(m_info.commandBufferCount);

    m_commandBuffers = m_info.device.allocateCommandBuffers({
      m_info.commandPool, // commandPool
      m_info.level, // level
      static_cast<uint32_t>(m_info.commandBufferCount) // commandBufferCount
      }
    );

    // set up begin info.
    m_beginInfo.flags = m_info.usageFlags;

    // Set up submit info.
    m_submitInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
    m_submitInfo.pCommandBuffers = m_commandBuffers.data();
  }

  void CmdBuffer::init(CmdBufferInfo&& info)
  {
    init(info);
  }

  void CmdBuffer::free()
  {
    m_info.device.freeCommandBuffers(m_info.commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
  }

  void CmdBuffer::reset()
  {
    for (vk::CommandBuffer& buffer : m_commandBuffers)
      buffer.reset(m_info.resetFlags);
  }

  void CmdBuffer::begin(size_t index)
  {
    m_commandBuffers[index].begin(m_beginInfo);
  }

  void CmdBuffer::end(size_t index)
  {
    m_commandBuffers[index].end();
  }

  void CmdBuffer::submitToQueue(const vk::Queue queue) const
  {
    if (m_info.usageFlags & vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    {
      if (queue.submit(1, &m_submitInfo, nullptr) != vk::Result::eSuccess)
        RX_ERROR("Failed to submit");

      queue.waitIdle();
    }
    else
      RX_ERROR("Only command buffers with a usage flag containing eOneTimeSubmit should be submitted automatically");
  }

  void CmdBuffer::setSubmitInfo(const vk::SubmitInfo& submitInfo)
  {
    m_submitInfo = submitInfo;
  }
}