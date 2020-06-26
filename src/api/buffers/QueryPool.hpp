#ifndef QUERY_POOL_HPP
#define QUERY_POOL_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct QueryPoolInfo
  {
    vk::Device device;
    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
    uint32_t queryCount;
    vk::QueryType queryType;
  };

  class QueryPool
  {
  public:
    QueryPool() = default;
    QueryPool(QueryPoolInfo& info);
    ~QueryPool();
     
    inline vk::QueryPool get() { return m_queryPool; }

    void init(QueryPoolInfo& info);
    void destroy();

  private:
    vk::QueryPool m_queryPool;
    QueryPoolInfo m_info;
  };
}

#endif // QUERY_POOL_HPP