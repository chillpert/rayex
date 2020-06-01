#include "CommandPool.hpp"

namespace RX
{
  CommandPool::~CommandPool()
  {
    destroy();
  }

	void CommandPool::initialize(CommandPoolInfo& info)
  {
    m_info = info;

    VkCommandPoolCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = m_info.createFlags;
    createInfo.queueFamilyIndex = m_info.queueFamilyIndex;

    VK_CREATE(vkCreateCommandPool(m_info.device, &createInfo, nullptr, &m_commandPool), "command pool from family index: " + std::to_string(m_info.queueFamilyIndex));
  }

  void CommandPool::destroy()
  {
    VK_DESTROY(vkDestroyCommandPool(m_info.device, m_commandPool, nullptr), "command pool");
    m_commandPool = VK_NULL_HANDLE;
  }

  void CommandPool::reset()
  {
    VK_ASSERT(vkResetCommandPool(m_info.device, m_commandPool, m_info.resetFlags), "Failed to reset command pool.");
  }
}