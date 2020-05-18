#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class Semaphore : public BaseComponent
  {
  public:
    Semaphore();
    ~Semaphore();

    inline VkSemaphore& get() { return m_semaphore; }

    void initialize(VkDevice device);
    void destroy();

  private:
    VkSemaphore m_semaphore;
    VkDevice m_device;
  };
}

#endif // SEMAPHORE_HPP