#include "base/LightNode.hpp"

namespace RENDERER_NAMESPACE
{
  glm::vec4 combine( const glm::vec3& vec3, float value )
  {
    return { vec3.x, vec3.y, vec3.z, value };
  }

  DirectionalLightNode::Ubo DirectionalLightNode::toUbo( )
  {
    Ubo ubo;
    ubo.ambient = combine( this->ambient, this->ambientIntensity );
    ubo.diffuse = combine( this->diffuse, this->diffuseIntensity );
    ubo.specular = combine( this->specular, this->specularIntensity );
    ubo.direction = combine( this->direction, this->exists );

    return ubo;
  }

  PointLightNode::Ubo PointLightNode::toUbo( )
  {
    Ubo ubo;
    ubo.ambient = combine( this->ambient, this->ambientIntensity );
    ubo.diffuse = combine( this->diffuse, this->diffuseIntensity );
    ubo.specular = combine( this->specular, this->specularIntensity );
    ubo.position = combine( this->position, this->exists );

    ubo.constant = this->constant;
    ubo.linear = this->linear;
    ubo.quadratic = this->quadratic;

    return ubo;
  }
}