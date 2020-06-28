#include "DescriptorSet.hpp"

namespace RX
{
  DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayoutInfo& info)
  {
    init(info);
  }

  DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayoutInfo&& info)
  {
    init(info);
  }

  DescriptorSetLayout::~DescriptorSetLayout()
  {
    if (m_layout)
      destroy();
  }

  void DescriptorSetLayout::addBinding(const vk::DescriptorSetLayoutBinding& binding)
  {
    if (m_layout)
      RX_ERROR("Failed to add binding because the descriptor set layout was already initialized.");

    m_bindings.push_back(binding);
  }

  void DescriptorSetLayout::clearBindings()
  {
    m_bindings.clear();
  }

  void DescriptorSetLayout::init(DescriptorSetLayoutInfo& info)
  {
    m_info = info;

    vk::DescriptorSetLayoutCreateInfo createInfo;
    createInfo.bindingCount = static_cast<uint32_t>(m_bindings.size());
    createInfo.pBindings = m_bindings.data();
    //createInfo.flags = m_info.flags;

    m_layout = m_info.device.createDescriptorSetLayout(createInfo);
    if (!m_layout)
      RX_ERROR("Failed to create descriptor set layout.");
  }

  void DescriptorSetLayout::init(DescriptorSetLayoutInfo&& info)
  {
    init(info);
  }

  void DescriptorSetLayout::destroy()
  {
    m_info.device.destroyDescriptorSetLayout(m_layout);
    m_layout = nullptr;

    m_bindings.clear();
  }
}