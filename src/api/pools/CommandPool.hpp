#ifndef COMMAND_POOL_HPP
#define COMMAND_POOL_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class CommandPool
  {
  public:
    ~CommandPool();
    
    inline VkCommandPool get() { return m_commandPool; }

    void initialize(VkDevice device, uint32_t queueFamilyIndex);
    void destroy();

    void reset(VkCommandPoolResetFlags flags = 0);

  private:
    VkCommandPool m_commandPool;
    VkDevice m_device;

    bool m_created = false;
  };
}

#endif // COMMAND_POOL_HPP