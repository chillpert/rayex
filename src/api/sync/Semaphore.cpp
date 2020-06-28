#include "Semaphore.hpp"

namespace RX
{
  Semaphore::Semaphore(SemaphoreInfo& info)
  {
    init(info);
  }

  Semaphore::Semaphore(SemaphoreInfo&& info)
  {
    init(info);
  }

  Semaphore::~Semaphore()
  {
    if (m_semaphore)
      destroy();
  }

  void Semaphore::init(SemaphoreInfo& info)
  {
    m_info = info;

    vk::SemaphoreCreateInfo createInfo;

    m_semaphore = m_info.device.createSemaphore(createInfo);
    if (!m_semaphore)
      RX_ERROR("Failed to create semaphore.");
  }

  void Semaphore::init(SemaphoreInfo&& info)
  {
    init(info);
  }

  void Semaphore::destroy()
  {
    m_info.device.destroySemaphore(m_semaphore);
    m_semaphore = nullptr;
  }
}