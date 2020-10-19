#include "base/Lights.hpp"

namespace RAYEX_NAMESPACE
{
  auto directionalLightInstance( const glm::vec3& direction ) -> std::shared_ptr<DirectionalLight>
  {
    auto directionalLight = std::make_shared<DirectionalLight>( );
    directionalLight->direction = direction;

    return directionalLight;
  }

  auto pointLightInstance( const glm::vec3& position ) -> std::shared_ptr<PointLight>
  {
    auto pointLight = std::make_shared<PointLight>( );
    pointLight->position = position;

    return pointLight;
  }
}