#include "CommandPool.hpp"

namespace RX
{
  CommandPool::CommandPool(CommandPoolInfo& info)
  {
    init(info);
  }

  CommandPool::CommandPool(CommandPoolInfo&& info)
  {
    init(info);
  }

  CommandPool::~CommandPool()
  {
    if (m_commandPool)
      destroy();
  }

	void CommandPool::init(CommandPoolInfo& info)
  {
    m_info = info;

    vk::CommandPoolCreateInfo createInfo;
    createInfo.flags = m_info.createFlags;
    createInfo.queueFamilyIndex = m_info.queueFamilyIndex;

    m_commandPool = m_info.device.createCommandPool(createInfo);
    if (!m_commandPool)
      RX_ERROR("Failed to create command pool");
  }
  
  void CommandPool::init(CommandPoolInfo&& info)
  {
    init(info);
  }

  void CommandPool::destroy()
  {
    m_info.device.destroyCommandPool(m_commandPool);
    m_commandPool = nullptr;
  }

  void CommandPool::reset()
  {
    m_info.device.resetCommandPool(m_commandPool, m_info.resetFlags);
  }
}