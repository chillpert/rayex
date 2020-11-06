#pragma once

#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
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

  /// A commodity function for allocating a directional light instance.
  /// @param direction The directional lights' direction.
  /// @return Returns a pointer to a directional light instance.
  /// @ingroup BASE
  RX_API auto directionalLightInstance( const glm::vec3& direction ) -> std::shared_ptr<DirectionalLight>;

  /// A commodity function for allocating a point light instance.
  /// @param position The point lights' position.
  /// @return Returns a pointer to a point light instance.
  /// @ingroup BASE
  RX_API auto pointLightInstance( const glm::vec3& position ) -> std::shared_ptr<PointLight>;
} // namespace RAYEX_NAMESPACE
