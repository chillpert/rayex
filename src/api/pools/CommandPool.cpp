#include "CommandPool.hpp"
#include "Components.hpp"

namespace RX
{
  CommandPool::CommandPool(uint32_t queueFamilyIndex, vk::CommandPoolCreateFlags createFlags, vk::CommandPoolResetFlags resetFlags, bool initialize)
  {
    if (initialize)
      init(queueFamilyIndex, createFlags, resetFlags);
  }

	void CommandPool::init(uint32_t queueFamilyIndex, vk::CommandPoolCreateFlags createFlags, vk::CommandPoolResetFlags resetFlags)
  {
    m_resetFlags = resetFlags;

    vk::CommandPoolCreateInfo createInfo;
    createInfo.flags = createFlags;
    createInfo.queueFamilyIndex = queueFamilyIndex;

    m_commandPool = g_device.createCommandPoolUnique(createInfo);
    if (!m_commandPool)
      RX_ERROR("Failed to create command pool.");
  }

  void CommandPool::destroy()
  {

  }

  void CommandPool::reset()
  {
    g_device.resetCommandPool(m_commandPool.get(), m_resetFlags);
  }
}