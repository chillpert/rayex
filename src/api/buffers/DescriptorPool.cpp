#include "DescriptorPool.hpp"

namespace RX
{
  DescriptorPool::DescriptorPool() :
    BaseComponent("DescriptorPool") { }

  DescriptorPool::~DescriptorPool()
  {
    destroy();
  }

  void DescriptorPool::initialize(VkDevice device, size_t swapchainImagesCount)
  {
    m_device = device;

    VkDescriptorPoolSize poolSize{ };
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(swapchainImagesCount);

    VkDescriptorPoolCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = 1;
    createInfo.pPoolSizes = &poolSize;
    createInfo.maxSets = static_cast<uint32_t>(swapchainImagesCount);

    VK_ASSERT(vkCreateDescriptorPool(device, &createInfo, nullptr, &m_pool), "Failed to create descriptor pool.");

    RX_INITIALIZATION_CALLBACK;
  }

  void DescriptorPool::destroy()
  {
    RX_ASSERT_DESTRUCTION;
    vkDestroyDescriptorPool(m_device, m_pool, nullptr);
  }
}