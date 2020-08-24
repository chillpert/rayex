#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "stdafx.hpp"

namespace rx
{
  class RX_API Settings
  {
  public:
    static void setMaxRecursionDepth( uint32_t maxRecursionDepth );
    inline static uint32_t getMaxRecursionDepth( ) { return m_maxRecursionDepth; }

    static bool refresh( ) { return m_refresh; }

  private:
    static uint32_t m_maxRecursionDepth;
    static bool m_refresh;
  };
}

#endif // SETTINGS_HPP