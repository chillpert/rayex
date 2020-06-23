#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct CommandPoolInfo
  {
    vk::Device device;
    uint32_t queueFamilyIndex;
    vk::CommandPoolCreateFlags createFlags;
    vk::CommandPoolResetFlags resetFlags;
  };

  class CommandPool
  {
  public:
    CommandPool() = default;
    CommandPool(CommandPoolInfo& info);
    ~CommandPool();
    
    inline vk::CommandPool get() { return m_commandPool; }

    void initialize(CommandPoolInfo& info);
    void destroy();

    // All command buffers allocated from this pool will return to their initial state.
    // All recording that might have been done will disappear.
    void reset();

  private:
    vk::CommandPool m_commandPool;
    CommandPoolInfo m_info;
  };
}

#endif // COMMAND_POOL_HPP