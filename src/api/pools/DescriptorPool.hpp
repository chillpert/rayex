#ifndef DESCRIPTOR_POOL
#define DESCRIPTOR_POOL

#include "pch/stdafx.hpp"

namespace RX
{
  struct DescriptorPoolInfo
  {
    VkDevice device;
    std::vector<VkDescriptorPoolSize> poolSizes;
    uint32_t maxSets;
  };

  class DescriptorPool
  {
  public:
    RX_API ~DescriptorPool();

    inline VkDescriptorPool get() { return m_pool; }
    inline DescriptorPoolInfo& getInfo() { return m_info; }

    void initialize(DescriptorPoolInfo& info);
    void destroy();

  private:
    VkDescriptorPool m_pool;
    DescriptorPoolInfo m_info;

    bool m_created = false;
  };
}

#endif // DESCRIPTOR_POOL