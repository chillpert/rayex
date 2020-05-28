#ifndef DESCRIPTOR_POOL
#define DESCRIPTOR_POOL

#include "pch/stdafx.hpp"

namespace RX
{
  class DescriptorPool
  {
  public:
    RX_API ~DescriptorPool();

    inline VkDescriptorPool get() { return m_pool; }

    void initialize(VkDevice device, size_t swapchainImagesCount);
    void destroy();

  private:
    VkDescriptorPool m_pool;
    VkDevice m_device;

    bool m_created = false;
  };
}

#endif // DESCRIPTOR_POOL