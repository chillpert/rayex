#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class DescriptorSetLayout : public BaseComponent
  {
  public:
    DescriptorSetLayout();
    ~DescriptorSetLayout();

    inline VkDescriptorSetLayout get() { return m_layout; }

    // Creates the layout of the descriptor set.
    void initialize(VkDevice device);
    void destroy();

  private:
    VkDescriptorSetLayout m_layout;
    VkDevice m_device;
  };
}

#endif // DESCRIPTOR_SET_LAYOUT_HPP