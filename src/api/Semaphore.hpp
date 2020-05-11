#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Semaphore
  {
  public:
    inline VkSemaphore get() { return semaphore; }

    void create(VkDevice device)
    {
      VkSemaphoreCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VK_ASSERT(vkCreateSemaphore(device, &createInfo, nullptr, &semaphore), "Failed to create semaphore");
      
    }

    void destroy(VkDevice device)
    {
      vkDestroySemaphore(device, semaphore, nullptr);
    }

  private:
    VkSemaphore semaphore;
  };
}

#endif // SEMAPHORE_HPP