#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct DescriptorSetLayoutInfo
  {
    vk::Device device;
    std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;
  };

  class DescriptorSetLayout
  {
  public:
    ~DescriptorSetLayout();

    inline vk::DescriptorSetLayout get() { return m_layout; }
    inline DescriptorSetLayoutInfo getInfo() { return m_info; }

    // Creates the layout of the descriptor set.
    void initialize(DescriptorSetLayoutInfo& info);
    void destroy();

  private:
    vk::DescriptorSetLayout m_layout;
    DescriptorSetLayoutInfo m_info;

    bool m_created = false;
  };
}

#endif // DESCRIPTOR_SET_LAYOUT_HPP