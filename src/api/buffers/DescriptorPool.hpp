#ifndef DESCRIPTOR_POOL
#define DESCRIPTOR_POOL

#include "api/BaseComponent.hpp"

namespace RX
{
  class DescriptorPool : public BaseComponent
  {
  public:
    RX_API DescriptorPool();
    RX_API ~DescriptorPool();

    inline VkDescriptorPool get() { return m_pool; }

    void initialize(VkDevice device, size_t swapchainImagesCount);
    void destroy();

  private:
    VkDescriptorPool m_pool;
    VkDevice m_device;
  };
}

#endif // DESCRIPTOR_POOL