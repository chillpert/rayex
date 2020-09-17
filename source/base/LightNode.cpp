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
    ubo.m_ambient = combine( m_ambient, m_ambientIntensity );
    ubo.m_diffuse = combine( m_diffuse, m_diffuseIntensity );
    ubo.m_specular = combine( m_specular, m_specularIntensity );
    ubo.m_direction = combine( m_direction, m_exists );

    return ubo;
  }

  PointLightNode::Ubo PointLightNode::toUbo( )
  {
    Ubo ubo;
    ubo.m_ambient = combine( m_ambient, m_ambientIntensity );
    ubo.m_diffuse = combine( m_diffuse, m_diffuseIntensity );
    ubo.m_specular = combine( m_specular, m_specularIntensity );
    ubo.m_position = combine( m_position, m_exists );

    ubo.m_constant = m_constant;
    ubo.m_linear = m_linear;
    ubo.m_quadratic = m_quadratic;

    return ubo;
  }
}