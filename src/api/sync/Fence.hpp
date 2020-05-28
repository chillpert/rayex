#ifndef FENCE_HPP
#define FENCE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Fence
  {
  public:
    ~Fence();

    inline VkFence& get() { return m_fence; }

    void initialize(VkDevice device);
    void destroy();

  private:
    VkFence m_fence = VK_NULL_HANDLE;
    VkDevice m_device;

    bool m_created = false;
  };
}


#endif