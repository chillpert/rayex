#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A common base for all light types.
  ///
  /// Instances of this type should not be used anywhere.
  /// @ingroup BASE
  struct Light
  {
    glm::vec3 ambient  = glm::vec3( 1.0F ); ///< The ambient color.
    glm::vec3 diffuse  = glm::vec3( 1.0F ); ///< The diffuse color.
    glm::vec3 specular = glm::vec3( 1.0F ); ///< The specular color.

    float ambientIntensity  = 1.0F; ///< The intensity of the ambient color.
    float diffuseIntensity  = 1.0F; ///< The intensity of the diffuse color.
    float specularIntensity = 1.0F; ///< The intensity of the specular color.

    bool update = true; ///< If set to true the light will be re-uploaded to the physical device.
  };

  /// A directional light.
  /// @ingroup BASE
  struct DirectionalLight : public Light
  {
    glm::vec3 direction = glm::vec3( 1.0F );
  };

  /// A point light.
  /// @ingroup BASE
  struct PointLight : public Light
  {
    glm::vec3 position = glm::vec3( 1.0F );
  };
} // namespace RAYEXEC_NAMESPACE

#endif // LIGHTS_HPP
