#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include "Buffer.hpp"

namespace rx
{
  struct UniformBufferObject
  {
    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 view = glm::mat4( 1.0f );
    glm::mat4 projection = glm::mat4( 1.0f );
    glm::vec3 cameraPos = glm::vec3( 1.0f );
  };

  class UniformBuffer
  {
  public:
    UniformBuffer( ) = default;
    UniformBuffer( size_t swapchainImagesCount, bool initialize = true );

    inline const std::vector<Buffer>& get( ) const { return m_buffers; }
    const std::vector<vk::Buffer> getRaw( ) const;

    void init( size_t swapchainImagesCount );

    void upload( uint32_t imageIndex, UniformBufferObject& ubo );

  private:
    std::vector<Buffer> m_buffers;
  };

}

#endif // UNIFORM_BUFFER_HPP