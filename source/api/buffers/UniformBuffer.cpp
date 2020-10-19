#include "api/buffers/UniformBuffer.hpp"

#include "api/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  auto UniformBuffer::getRaw( ) const -> const std::vector<vk::Buffer>
  {
    std::vector<vk::Buffer> res( this->buffers.size( ), nullptr );

    for ( size_t i = 0; i < this->buffers.size( ); ++i )
    {
      res[i] = this->buffers[i].get( );
    }

    return res;
  }

  DirectionalLightUbo directionalLightToUbo( std::shared_ptr<DirectionalLight> light )
  {
    RX_ASSERT( light != nullptr, "Invalid directional light handle." );

    return DirectionalLightUbo { glm::vec4( light->ambient, light->ambientIntensity ),
                                 glm::vec4( light->diffuse, light->diffuseIntensity ),
                                 glm::vec4( light->specular, light->specularIntensity ),
                                 glm::vec4( light->direction, 1.0F ) };
  }
} // namespace RAYEXEC_NAMESPACE
