#ifndef DESCRIPTOR_SET
#define DESCRIPTOR_SET

#include "api/BaseComponent.hpp"

namespace RX
{
  class DescriptorSet : public BaseComponent
  {
  public:
    DescriptorSet();
    ~DescriptorSet();

    inline VkDescriptorSetLayout get() { return m_descriptorSetLayout; }

    // Creates the layout of the descriptor set.
    void initialize(VkDevice device);
    void destroy();

  private:
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkDevice m_device;
  };
}

#endif // DESCRIPTOR_SET