#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include "Buffer.hpp"

namespace rx
{
  struct CameraUbo
  {
    glm::mat4 view = glm::mat4( 1.0f );
    glm::mat4 projection = glm::mat4( 1.0f );
    glm::mat4 viewInverse = glm::mat4( 1.0f );
    glm::mat4 projectionInverse = glm::mat4( 1.0f );
  };

  struct MvpUbo
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
    
    template <typename T>
    UniformBuffer( size_t swapchainImagesCount, bool initialize = true )
    {
      if ( initialize )
        init( swapchainImagesCount );
    }

    inline const std::vector<Buffer>& get( ) const { return m_buffers; }
    const std::vector<vk::Buffer> getRaw( ) const;

    template <typename T>
    void init( size_t swapchainImagesCount )
    {
      m_buffers.resize( swapchainImagesCount );

      for ( Buffer& buffer : m_buffers )
      {
        buffer.init( sizeof( T ),
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     { },
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
      }
    }

    template <typename T>
    void upload( uint32_t imageIndex, T& ubo )
    {
      m_buffers[imageIndex].fill<T>( &ubo );
    }

  private:
    std::vector<Buffer> m_buffers;
  };

}

#endif // UNIFORM_BUFFER_HPP