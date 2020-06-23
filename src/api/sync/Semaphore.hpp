#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct SemaphoreInfo
  {
    vk::Device device;
  };

  class Semaphore
  {
  public:
    ~Semaphore();

    inline vk::Semaphore& get() { return m_semaphore; }

    void initialize(SemaphoreInfo& info);
    void destroy();

  private:
    vk::Semaphore m_semaphore;
    SemaphoreInfo m_info;
  };
}

#endif // SEMAPHORE_HPP