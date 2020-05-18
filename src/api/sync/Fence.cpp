#include "Fence.hpp"

namespace RX
{
  Fence::Fence() :
    BaseComponent("Fence") { }

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

    initializationCallback();
  }

  void Fence::destroy()
  {
    assertDestruction();
    VK_ASSERT(vkDestroyFence(m_device, m_fence, nullptr), "Failed to destroy fence.");
  }
}