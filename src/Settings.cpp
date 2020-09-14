#include "Settings.hpp"

namespace RENDERER_NAMESPACE
{
  uint32_t Settings::s_maxRecursionDepth = 8;
  bool Settings::s_refresh = false;

  void Settings::setMaxRecursionDepth( uint32_t maxRecursionDepth )
  {
    s_maxRecursionDepth = maxRecursionDepth;
    s_refresh = true;
  }
}