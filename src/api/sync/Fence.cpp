#include "Fence.hpp"

namespace RX
{
  Fence::~Fence()
  {
    destroy();
  }

  void Fence::initialize(FenceInfo& info)
  {
    m_info = info;

    VkFenceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_CREATE(vkCreateFence(m_info.device, &createInfo, nullptr, &m_fence), "fence");
  }

  void Fence::destroy()
  {
    VK_DESTROY(vkDestroyFence(m_info.device, m_fence, nullptr), "fence");
    m_fence = VK_NULL_HANDLE;
  }
}