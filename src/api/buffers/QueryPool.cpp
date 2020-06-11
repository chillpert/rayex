#include "QueryPool.hpp"

namespace RX
{
  QueryPool::QueryPool(QueryPoolInfo& info)
  {
    initialize(info);
  }

  QueryPool::~QueryPool()
  {
    if (m_queryPool)
      destroy();
  }

  void QueryPool::initialize(QueryPoolInfo& info)
  {
    m_info = info;

    vk::QueryPoolCreateInfo createInfo;
    createInfo.queryCount = m_info.queryCount;
    createInfo.queryType = m_info.queryType;

    m_queryPool = m_info.device.createQueryPool(createInfo, nullptr, m_info.dispatchLoaderDynamic);
  }

  void QueryPool::destroy()
  {
    m_info.device.destroyQueryPool(m_queryPool, nullptr, m_info.dispatchLoaderDynamic);
  }
}