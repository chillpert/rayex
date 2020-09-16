#include "Settings.hpp"

namespace RENDERER_NAMESPACE
{
  uint32_t Settings::s_maxRecursionDepth = 8;
  bool Settings::s_refresh = false;
  glm::vec4 Settings::s_clearColor = glm::vec4( 0.4f, 0.4f, 0.4f, 1.0f );
}