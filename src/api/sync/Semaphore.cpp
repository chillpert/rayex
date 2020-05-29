#include "Semaphore.hpp"

namespace RX
{
  Semaphore::~Semaphore()
  {
    destroy();
  }

  void Semaphore::initialize(SemaphoreInfo& info)
  {
    m_info = info;

    VkSemaphoreCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VK_CREATE(vkCreateSemaphore(m_info.device, &createInfo, nullptr, &m_semaphore), "semaphore");
  }

  void Semaphore::destroy()
  {
    VK_DESTROY(vkDestroySemaphore(m_info.device, m_semaphore, nullptr), "semaphore");
    m_semaphore = VK_NULL_HANDLE;
  }
}