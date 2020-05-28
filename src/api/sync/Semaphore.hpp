#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Semaphore
  {
  public:
    ~Semaphore();

    inline VkSemaphore& get() { return m_semaphore; }

    void initialize(VkDevice device);
    void destroy();

  private:
    VkSemaphore m_semaphore;
    VkDevice m_device;

    bool m_created = false;
  };
}

#endif // SEMAPHORE_HPP