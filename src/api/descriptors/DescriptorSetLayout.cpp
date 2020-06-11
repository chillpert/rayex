#include "DescriptorSet.hpp"

namespace RX
{
  DescriptorSetLayout::~DescriptorSetLayout()
  {
    if (m_layout)
      destroy();
  }

  void DescriptorSetLayout::initialize(DescriptorSetLayoutInfo& info)
  {
    m_info = info;

    vk::DescriptorSetLayoutCreateInfo createInfo;
    createInfo.bindingCount = static_cast<uint32_t>(m_info.layoutBindings.size());
    createInfo.pBindings = m_info.layoutBindings.data();

    m_layout = m_info.device.createDescriptorSetLayout(createInfo);
    if (!m_layout)
      RX_ERROR("Failed to create descriptor set layout.");
  }

  void DescriptorSetLayout::destroy()
  {
    m_info.device.destroyDescriptorSetLayout(m_layout);
    m_layout = nullptr;
  }
}