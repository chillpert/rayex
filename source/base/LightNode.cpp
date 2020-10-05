#include "base/LightNode.hpp"

namespace RAYEXEC_NAMESPACE
{
  auto combine( const glm::vec3& vec3, float value ) -> glm::vec4
  {
    return { vec3.x, vec3.y, vec3.z, value };
  }

  auto DirectionalLightNode::toUbo( ) -> DirectionalLightNode::Ubo
  {
    return DirectionalLightNode::Ubo { combine( this->ambient, this->ambientIntensity ),
                                       combine( this->diffuse, this->diffuseIntensity ),
                                       combine( this->specular, this->specularIntensity ),
                                       combine( this->position, this->exists ) };
  }

  auto PointLightNode::toUbo( ) -> PointLightNode::Ubo
  {
    return PointLightNode::Ubo { combine( this->ambient, this->ambientIntensity ),
                                 combine( this->diffuse, this->diffuseIntensity ),
                                 combine( this->specular, this->specularIntensity ),
                                 combine( this->position, this->exists ),
                                 this->constant,
                                 this->linear,
                                 this->quadratic };
  }
} // namespace RAYEXEC_NAMESPACE