#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class CommandPool
  {
  public:
    inline VkCommandPool get() { return commandPool; }

    void create(VkDevice device, uint32_t* queueFamilyIndex)
    {
      VkCommandPoolCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
      createInfo.queueFamilyIndex = *queueFamilyIndex;

      VK_ASSERT(vkCreateCommandPool(device, &createInfo, nullptr, &commandPool), "Failed to create command pool");
    }

    void destroy(VkDevice device)
    {
      vkDestroyCommandPool(device, commandPool, nullptr);
    }

    void reset(VkDevice device, VkCommandPoolResetFlags flags = 0)
    {
      VK_ASSERT(vkResetCommandPool(device, commandPool, flags), "Failed to reset command pool");
    }

  private:
    VkCommandPool commandPool;
  };
}

#endif // COMMAND_POOL_HPP