#include "Fence.hpp"

namespace RX
{
  Fence::~Fence()
  {
    if (m_fence)
      destroy();
  }

  void Fence::init(FenceInfo& info)
  {
    m_info = info;

    vk::FenceCreateInfo createInfo;
    createInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    m_fence = m_info.device.createFence(createInfo);
    if (!m_fence)
      RX_ERROR("Failed to create fence.");
  }

  void Fence::destroy()
  {
    m_info.device.destroyFence(m_fence);
    m_fence = nullptr;
  }
}