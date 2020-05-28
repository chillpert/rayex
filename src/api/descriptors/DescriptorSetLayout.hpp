#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class DescriptorSetLayout
  {
  public:
    ~DescriptorSetLayout();

    inline VkDescriptorSetLayout get() { return m_layout; }

    // Creates the layout of the descriptor set.
    void initialize(VkDevice device);
    void destroy();

  private:
    VkDescriptorSetLayout m_layout;
    VkDevice m_device;

    bool m_created = false;
  };
}

#endif // DESCRIPTOR_SET_LAYOUT_HPP