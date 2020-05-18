#ifndef FENCE_HPP
#define FENCE_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class Fence : public BaseComponent
  {
  public:
    Fence();
    ~Fence();

    inline VkFence get() { return m_fence; }

    void initialize(VkDevice device);
    void destroy();

  private:
    VkFence m_fence;
    VkDevice m_device;
  };
}


#endif