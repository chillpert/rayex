#include "QueryPool.hpp"

namespace RX
{
  QueryPool::QueryPool(QueryPoolInfo& info)
  {
    init(info);
  }
  
  QueryPool::QueryPool(QueryPoolInfo&& info)
  {
    init(info);
  }

  QueryPool::~QueryPool()
  {
    if (m_queryPool)
      destroy();
  }

  void QueryPool::init(QueryPoolInfo& info)
  {
    m_info = info;

    vk::QueryPoolCreateInfo createInfo{
      { },                // flags
      m_info.queryType,   // queryType
      m_info.queryCount,  // queryCount
      { }                 // pipelineStatistics
    };

    m_queryPool = m_info.device.createQueryPool(createInfo, nullptr, m_info.dispatchLoaderDynamic);
    if (!m_queryPool)
      RX_ERROR("Failed to create query pool");
  }

  void QueryPool::init(QueryPoolInfo&& info)
  {
    init(info);
  }

  void QueryPool::destroy()
  {
    m_info.device.destroyQueryPool(m_queryPool, nullptr, m_info.dispatchLoaderDynamic);
  }
}