#include "DescriptorPool.hpp"

namespace RX
{
  DescriptorPool::~DescriptorPool()
  {
    destroy();
  }

  void DescriptorPool::initialize(DescriptorPoolInfo& info)
  {
    m_info = info;

    std::vector< VkDescriptorPoolSize> poolSizes(m_info.types.size());

    for (size_t i = 0; i < m_info.types.size(); ++i)
    {
      poolSizes[i].type = m_info.types[i];
      poolSizes[i].descriptorCount = static_cast<uint32_t>(m_info.swapchainImagesCount);
    }

    VkDescriptorPoolCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    createInfo.pPoolSizes = poolSizes.data();
    createInfo.maxSets = m_info.maxSets;

    VK_CREATE(vkCreateDescriptorPool(m_info.device, &createInfo, nullptr, &m_pool), "descriptor pool");
  }

  void DescriptorPool::destroy()
  {
    VK_DESTROY(vkDestroyDescriptorPool(m_info.device, m_pool, nullptr), "descriptor pool");
    m_pool = VK_NULL_HANDLE;
  }
}