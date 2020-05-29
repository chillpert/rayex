#ifndef FENCE_HPP
#define FENCE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct FenceInfo
  {
    VkDevice device;
  };

  class Fence
  {
  public:
    ~Fence();

    inline VkFence& get() { return m_fence; }
    inline FenceInfo& getInfo() { return m_info; }

    void initialize(FenceInfo& info);
    void destroy();

  private:
    VkFence m_fence = VK_NULL_HANDLE;
    FenceInfo m_info;

    bool m_created = false;
  };
}


#endif