#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct DescriptorSetLayoutInfo
  {
    vk::Device device;
  };

  class DescriptorSetLayout
  {
  public:
    ~DescriptorSetLayout();

    inline vk::DescriptorSetLayout get() { return m_layout; }

    void addBinding(const vk::DescriptorSetLayoutBinding& binding);
    void clearBindings();

    // Creates the layout of the descriptor set.
    void init(DescriptorSetLayoutInfo& info);
    void init(DescriptorSetLayoutInfo&& info);
    void destroy();

  private:
    vk::DescriptorSetLayout m_layout;
    DescriptorSetLayoutInfo m_info;

    std::vector<vk::DescriptorSetLayoutBinding> m_bindings;
  };
}

#endif // DESCRIPTOR_SET_LAYOUT_HPP