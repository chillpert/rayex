#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class CommandPool
  {
  public:
    CommandPool() = default;
    CommandPool(uint32_t queueFamilyIndex, vk::CommandPoolCreateFlags createFlags, vk::CommandPoolResetFlags resetFlags, bool initialize = true);
    
    inline vk::CommandPool get() { return m_commandPool.get(); }

    void init(uint32_t queueFamilyIndex, vk::CommandPoolCreateFlags createFlags, vk::CommandPoolResetFlags resetFlags);
    void destroy();

    void reset();

  private:
    vk::UniqueCommandPool m_commandPool;

    vk::CommandPoolResetFlags m_resetFlags;
  };
}

#endif // COMMAND_POOL_HPP