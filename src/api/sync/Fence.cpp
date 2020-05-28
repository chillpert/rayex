#include "Fence.hpp"

namespace RX
{
  Fence::Fence() :
    BaseComponent("Fence"),
    m_fence(VK_NULL_HANDLE) { }

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

    VK_ASSERT(vkCreateFence(device, &createInfo, nullptr, &m_fence), "Failed to create fence.");

    RX_INITIALIZATION_CALLBACK;
  }

  void Fence::destroy()
  {
    RX_DESTROY(vkDestroyFence(m_device, m_fence, nullptr), "fence");
  }
}