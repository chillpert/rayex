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
    ~Fence();

    inline vk::Fence& get() { return m_fence; }
    inline FenceInfo& getInfo() { return m_info; }

    void initialize(FenceInfo& info);
    void destroy();

  private:
    vk::Fence m_fence = nullptr;
    FenceInfo m_info;
  };
}


#endif