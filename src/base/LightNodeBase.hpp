#ifndef LIGHT_NODE_BASE_HPP
#define LIGHT_NODE_BASE_HPP

#include "NodeBase.hpp"

namespace rx
{
  /// Used to combine a 3D vector with a float to a vec4 to keep uniform member alignment in shaders.
  /// 
  /// The float will be placed inside the fourth component of the resulting 4D vector.
  /// @param vec3 The 3D vector to combine with a float.
  /// @param value The float to combine with a 3D vector.
  /// @return Returns the combination in a 4D vector..
  glm::vec4 combine( const glm::vec3& vec3, float value );

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

    float m_ambientIntensity = 0.7f; ///< The ambient intensity.
    float m_diffuseIntensity = 1.0f; ///< The diffuse intensity.
    float m_specularIntensity = 1.0f; ///< The specular intensity.
    float m_exists = 1.0f;
  };

  /// Can be used to add directional light sources to the scene.
  /// @ingroup Base
  class DirectionalLightNode : public LightNode
  {
  public:
    virtual ~DirectionalLightNode( ) = default;

    glm::vec3 m_direction = { 1.0f, -1.0f, 1.0f }; ///< The direction the light is pointing at.    

    /// Used to combine members of DirectionalLightNode to data types that will not cause uniform member alignment issues in shaders.
    struct Ubo
    {
      glm::vec4 m_ambient; ///< Encodes the ambient color in the first three entries and its intensity in the fourth entry.
      glm::vec4 m_diffuse; ///< Encodes the diffuse color in the first three entries and its intensity in the fourth entry.
      glm::vec4 m_specular; ///< Encodes the specular color in the first three entries and its intensity in the fourth entry.

      glm::vec4 m_direction; ///< Encodes the light's direction in the first three entires and whether or not it is activated in the fourth entry.
    };

    Ubo toUbo( );
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

    /// Used to combine members of PointLightNode to data types that will not cause uniform member alignment issues in shaders.
    struct Ubo
    {
      glm::vec4 m_ambient; ///< Encodes the ambient color in the first three entries and its intensity in the fourth entry.
      glm::vec4 m_diffuse; ///< Encodes the diffuse color in the first three entries and its intensity in the fourth entry.
      glm::vec4 m_specular; ///< Encodes the specular color in the first three entries and its intensity in the fourth entry.

      glm::vec4 m_position; ///< Encodes the light's position in the first three entires and whether or not it is activated in the fourth entry.

      float m_constant;
      float m_linear;
      float m_quadratic;
    };

    Ubo toUbo( );
  };
}

#endif // LIGHT_NODE_BASE_HPP