#include "Fence.hpp"

namespace RX
{
  Fence::~Fence()
  {
    destroy();
  }

  void Fence::initialize(VkDevice device)
  {
    m_device = device;

    VkFenceCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_CREATE(vkCreateFence(device, &createInfo, nullptr, &m_fence), "fence");
  }

  void Fence::destroy()
  {
    VK_DESTROY(vkDestroyFence(m_device, m_fence, nullptr), "fence");
  }
}