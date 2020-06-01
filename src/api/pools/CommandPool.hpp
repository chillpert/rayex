#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct CommandPoolInfo
  {
    VkDevice device;
    uint32_t queueFamilyIndex;
    VkCommandPoolCreateFlags createFlags = 0;
    VkCommandPoolResetFlags resetFlags = 0;
  };

  class CommandPool
  {
  public:
    ~CommandPool();
    
    inline VkCommandPool get() { return m_commandPool; }
    inline CommandPoolInfo& getInfo() { return m_info; }

    void initialize(CommandPoolInfo& info);
    void destroy();

    // All command buffers allocated from this pool will return to their initial state.
    // All recording that might have been done will disappear.
    void reset();

  private:
    VkCommandPool m_commandPool;
    CommandPoolInfo m_info;

    bool m_created = false;
  };
}

#endif // COMMAND_POOL_HPP