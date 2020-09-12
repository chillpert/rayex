#ifndef LIGHT_NODE_BASE_HPP
#define LIGHT_NODE_BASE_HPP

#include "NodeBase.hpp"

namespace rx
{
  /// Adds lighting-related information to the TransformNode parent class.
  /// @note This node only provides common data for its inherited classes and cannot be instantiated.
  /// @ingroup Base
  class LightNode : public TransformNode
  {
  public:
    virtual ~LightNode( ) = default;

    glm::vec3 m_ambient = { 0.2f, 0.2f, 0.2f }; ///< The ambient color.
    glm::vec3 m_diffuse = { 0.8f, 0.8f, 0.8f }; ///< The diffuse color.
    glm::vec3 m_specular = { 1.0f, 1.0f, 1.0f }; ///< The specular highlight's color.

    float m_ambientStrength = 0.7f; ///< The ambient intensity.
  };

  /// Can be used to add directional light sources to the scene.
  /// @ingroup Base
  class DirectionalLightNode : public LightNode
  {
  public:
    virtual ~DirectionalLightNode( ) = default;

    glm::vec3 m_direction = { 1.0f, -1.0f, 1.0f }; ///< The direction the light is pointing at.    
    float m_exists = 1.0f;

    struct Ubo
    {
      glm::vec3 m_ambient;
      glm::vec3 m_diffuse;
      glm::vec3 m_specular;
      float m_ambientStrength;

      glm::vec3 m_direction;
      float m_exists;
    };

    Ubo toUbo( ) { return { m_ambient, m_diffuse, m_specular, m_ambientStrength, m_direction, m_exists }; }
  };

  /// Can be used to add point light sources to the scene.
  /// @ingroup Base
  class PointLightNode : public LightNode
  {
  public:
    virtual ~PointLightNode( ) = default;

    glm::vec3 m_position = glm::vec3( 0.0f ); ///< The position of the light.

    float m_constant = 1.0f; ///< The constant factor of the light's abbreviation.
    float m_linear = 0.09f; ///< The linear factor of the light's abbreviation.
    float m_quadratic = 0.032f; ///< The quadratic factor of the light's abbreviation.

    struct Ubo
    {
      glm::vec3 m_ambient;
      glm::vec3 m_diffuse;
      glm::vec3 m_specular;
      float m_ambientStrength;

      glm::vec3 m_position;

      float m_constant;
      float m_linear;
      float m_quadratic;
    };

    Ubo toUbo( ) { return { m_ambient, m_diffuse, m_specular, m_ambientStrength, m_position, m_constant, m_linear, m_quadratic }; }
  };
}

#endif // LIGHT_NODE_BASE_HPP