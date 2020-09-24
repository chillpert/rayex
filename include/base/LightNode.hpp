#ifndef LIGHT_NODE_BASE_HPP
#define LIGHT_NODE_BASE_HPP

#include "Node.hpp"

namespace RAYEXEC_NAMESPACE
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

    glm::vec3 ambient = { 0.2f, 0.2f, 0.2f }; ///< The ambient color.
    glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f }; ///< The diffuse color.
    glm::vec3 specular = { 1.0f, 1.0f, 1.0f }; ///< The specular highlight's color.

    float ambientIntensity = 0.7f; ///< The ambient intensity.
    float diffuseIntensity = 1.0; ///< The diffuse intensity.
    float specularIntensity = 1.0f; ///< The specular intensity.
    float exists = 1.0f;
  };

  /// Can be used to add directional light sources to the scene.
  /// @ingroup Base
  class DirectionalLightNode : public LightNode
  {
  public:
    virtual ~DirectionalLightNode( ) = default;
    
    glm::vec3 position = { 5.0f, 5.0f, 2.0f }; ///< The position of the light.    

    /// Used to combine members of DirectionalLightNode to data types that will not cause uniform member alignment issues in shaders.
    struct Ubo
    {
      glm::vec4 ambient; ///< Encodes the ambient color in the first three entries and its intensity in the fourth entry.
      glm::vec4 diffuse; ///< Encodes the diffuse color in the first three entries and its intensity in the fourth entry.
      glm::vec4 specular; ///< Encodes the specular color in the first three entries and its intensity in the fourth entry.

      glm::vec4 position; ///< Encodes the light's position in the first three entries and whether or not it is activated in the fourth entry.
    };

    Ubo toUbo( );
  };

  /// Can be used to add point light sources to the scene.
  /// @ingroup Base
  class PointLightNode : public LightNode
  {
  public:
    virtual ~PointLightNode( ) = default;

    glm::vec3 position = glm::vec3( 0.0f ); ///< The position of the light.

    float constant = 1.0f; ///< The constant factor of the light's abbreviation.
    float linear = 0.09f; ///< The linear factor of the light's abbreviation.
    float quadratic = 0.032f; ///< The quadratic factor of the light's abbreviation.

    /// Used to combine members of PointLightNode to data types that will not cause uniform member alignment issues in shaders.
    struct Ubo
    {
      glm::vec4 ambient; ///< Encodes the ambient color in the first three entries and its intensity in the fourth entry.
      glm::vec4 diffuse; ///< Encodes the diffuse color in the first three entries and its intensity in the fourth entry.
      glm::vec4 specular; ///< Encodes the specular color in the first three entries and its intensity in the fourth entry.

      glm::vec4 position; ///< Encodes the light's position in the first three entires and whether or not it is activated in the fourth entry.

      float constant;
      float linear;
      float quadratic;
    };

    Ubo toUbo( );
  };
}

#endif // LIGHT_NODE_BASE_HPP