#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class CommandPool
  {
  public:
    inline VkCommandPool get() { return commandPool; }

    void create(VkDevice device, uint32_t queueFamilyIndex)
    {
      VkCommandPoolCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
      createInfo.queueFamilyIndex = queueFamilyIndex;

      VK_ASSERT(vkCreateCommandPool(device, &createInfo, nullptr, &commandPool), "Failed to create command pool.");

      created = true;
    }

    void destroy(VkDevice device)
    {
      if (created)
      {
        vkDestroyCommandPool(device, commandPool, nullptr);
        created = true;
      }
      else
        VK_ERROR("Can not destroy command pool because it was already destroyed or never created to begin with.");
    }

    void reset(VkDevice device, VkCommandPoolResetFlags flags = 0)
    {
#ifdef RX_DEBUG
      if (!created)
        VK_ERROR("Can not reset the command pool because it was already destroyed or never created to begin with.");
#endif
      VK_ASSERT(vkResetCommandPool(device, commandPool, flags), "Failed to reset command pool.");
    }

  private:
    VkCommandPool commandPool;

    bool created = false;
  };
}

#endif // COMMAND_POOL_HPP