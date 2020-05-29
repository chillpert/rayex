#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct SemaphoreInfo
  {
    VkDevice device;
  };

  class Semaphore
  {
  public:
    ~Semaphore();

    inline VkSemaphore& get() { return m_semaphore; }
    inline SemaphoreInfo& getInfo() { return m_info; }

    void initialize(SemaphoreInfo& info);
    void destroy();

  private:
    VkSemaphore m_semaphore;
    SemaphoreInfo m_info;

    bool m_created = false;
  };
}

#endif // SEMAPHORE_HPP