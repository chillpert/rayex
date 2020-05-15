#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Semaphore : public BaseComponent
  {
  public:
    Semaphore() :
      BaseComponent("Semaphore") { }

    inline VkSemaphore get() { return m_semaphore; }

    void initialize(VkDevice device)
    {
      VkSemaphoreCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VK_ASSERT(vkCreateSemaphore(device, &createInfo, nullptr, &m_semaphore), "Failed to create semaphore.");

      initializationCallback();
    }

    void destroy(VkDevice device)
    {
      assertDestruction();
      vkDestroySemaphore(device, m_semaphore, nullptr);
    }

  private:
    VkSemaphore m_semaphore;
  };
}

#endif // SEMAPHORE_HPP