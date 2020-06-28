#ifndef FENCE_HPP
#define FENCE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct FenceInfo
  {
    vk::Device device;
  };

  class Fence
  {
  public:
    Fence() = default;
    Fence(FenceInfo& info);
    Fence(FenceInfo&& info);
    ~Fence();

    inline vk::Fence& get() { return m_fence; }

    void init(FenceInfo& info);
    void init(FenceInfo&& info);

    void destroy();

  private:
    vk::Fence m_fence = nullptr;
    FenceInfo m_info;
  };
}


#endif