#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class CommandPool : public BaseComponent
  {
  public:
    CommandPool() :
      BaseComponent("CommandPool") { }

    inline VkCommandPool get() { return m_commandPool; }

    void initialize(VkDevice device, uint32_t queueFamilyIndex)
    {
      VkCommandPoolCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      //createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;//VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
      createInfo.queueFamilyIndex = queueFamilyIndex;

      VK_ASSERT(vkCreateCommandPool(device, &createInfo, nullptr, &m_commandPool), "Failed to create command pool.");

      initializationCallback();
    }

    void destroy(VkDevice device)
    {
      assertDestruction();
      vkDestroyCommandPool(device, m_commandPool, nullptr);
     }

    void reset(VkDevice device, VkCommandPoolResetFlags flags = 0)
    {
      assertInitialized("reset");
      VK_ASSERT(vkResetCommandPool(device, m_commandPool, flags), "Failed to reset command pool.");
    }

  private:
    VkCommandPool m_commandPool;
  };
}

#endif // COMMAND_POOL_HPP