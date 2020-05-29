#include "DescriptorSet.hpp"

namespace RX
{
  DescriptorSetLayout::~DescriptorSetLayout()
  {
    destroy();
  }

  void DescriptorSetLayout::initialize(DescriptorSetLayoutInfo& info)
  {
    m_info = info;

    VkDescriptorSetLayoutCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = static_cast<uint32_t>(m_info.layoutBindings.size());
    createInfo.pBindings = m_info.layoutBindings.data();
 
    VK_CREATE(vkCreateDescriptorSetLayout(m_info.device, &createInfo, nullptr, &m_layout), "descriptor set layout");
  }

  void DescriptorSetLayout::destroy()
  {
    VK_DESTROY(vkDestroyDescriptorSetLayout(m_info.device, m_layout, nullptr), "layout");
    m_layout = VK_NULL_HANDLE;
  }
}