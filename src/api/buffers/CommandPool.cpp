#include "CommandPool.hpp"

namespace RX
{
  CommandPool::~CommandPool()
  {
    destroy();
  }

	void CommandPool::initialize(VkDevice device, uint32_t queueFamilyIndex)
  {
    m_device = device;

    VkCommandPoolCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    //createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;//VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    createInfo.queueFamilyIndex = queueFamilyIndex;

    VK_CREATE(vkCreateCommandPool(device, &createInfo, nullptr, &m_commandPool), "command pool");
  }

  void CommandPool::destroy()
  {
    VK_DESTROY(vkDestroyCommandPool(m_device, m_commandPool, nullptr), "command pool");
   }

  void CommandPool::reset(VkCommandPoolResetFlags flags)
  {
    VK_ASSERT(vkResetCommandPool(m_device, m_commandPool, flags), "Failed to reset command pool.");
  }
}