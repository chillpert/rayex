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

    VkDescriptorPoolCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = static_cast<uint32_t>(m_info.poolSizes.size());
    createInfo.pPoolSizes = m_info.poolSizes.data();
    createInfo.maxSets = m_info.maxSets;

    VK_CREATE(vkCreateDescriptorPool(m_info.device, &createInfo, nullptr, &m_pool), "descriptor pool");
  }

  void DescriptorPool::destroy()
  {
    VK_DESTROY(vkDestroyDescriptorPool(m_info.device, m_pool, nullptr), "descriptor pool");
    m_pool = VK_NULL_HANDLE;
  }
}