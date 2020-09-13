#include "LightNodeBase.hpp"

namespace rx
{
  glm::vec4 combine( const glm::vec3& vec3, float value )
  {
    return { vec3.x, vec3.y, vec3.z, value };
  }
}