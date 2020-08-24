#include "Settings.hpp"

namespace rx
{
  uint32_t Settings::m_maxRecursionDepth = 8;
  bool Settings::m_refresh = false;

  void Settings::setMaxRecursionDepth( uint32_t maxRecursionDepth )
  {
    m_maxRecursionDepth = maxRecursionDepth;
    m_refresh = true;
  }
}