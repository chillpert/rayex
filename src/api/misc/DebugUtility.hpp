#ifndef DEBUG_UTILITY_HPP
#define DEBUG_UTILITY_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct DebugUtilInfo
  {
    vk::Device device;
  };

  class DebugUtility
  {
  public:
    DebugUtility() { };
    
    DebugUtility(DebugUtilInfo& info)
    {
      initialize(info);
    }
    
    void initialize(DebugUtilInfo& info)
    {
      m_info = info;
    }

    void setObjectName(const size_t object, const char* name, vk::ObjectType t, vk::DispatchLoaderDynamic dispatchLoaderDynamic)
    {
#ifdef RX_DEBUG
      //vk::DebugUtilsObjectNameInfoEXT objectNameInfo(t, object, name);
      //m_info.device.setDebugUtilsObjectNameEXT(objectNameInfo, m_info.dispatchLoaderDynamic);
#endif
    }

  private:
    DebugUtilInfo m_info;
  };
}

#endif // DEBUG_UTILITY_HPP