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

    std::array<VkDescriptorPoolSize, 2> poolSizes{ };
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchainImagesCount);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchainImagesCount);

    VkDescriptorPoolCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    createInfo.pPoolSizes = poolSizes.data();
    createInfo.maxSets = static_cast<uint32_t>(swapchainImagesCount);

    VK_ASSERT(vkCreateDescriptorPool(device, &createInfo, nullptr, &m_pool), "Failed to create descriptor pool.");

    RX_INITIALIZATION_CALLBACK;
  }

  void DescriptorPool::destroy()
  {
    RX_DESTROY(vkDestroyDescriptorPool(m_device, m_pool, nullptr), "descriptor pool");
  }
}