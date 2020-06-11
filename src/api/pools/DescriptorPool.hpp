#ifndef DESCRIPTOR_POOL
#define DESCRIPTOR_POOL

#include "pch/stdafx.hpp"

namespace RX
{
  struct DescriptorPoolInfo
  {
    vk::Device device;
    std::vector<vk::DescriptorPoolSize> poolSizes;
    uint32_t maxSets;
  };

  class DescriptorPool
  {
  public:
    RX_API ~DescriptorPool();

    inline vk::DescriptorPool get() { return m_pool; }
    inline DescriptorPoolInfo& getInfo() { return m_info; }

    void initialize(DescriptorPoolInfo& info);
    void destroy();

  private:
    vk::DescriptorPool m_pool;
    DescriptorPoolInfo m_info;
  };
}

#endif // DESCRIPTOR_POOL