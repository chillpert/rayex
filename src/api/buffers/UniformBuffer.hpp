#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include "Buffer.hpp"
#include "LightNodeBase.hpp"

namespace rx
{
  /// A uniform buffer object for camera data.
  /// @ingroup API
  struct CameraUbo
  {
    glm::mat4 m_view = glm::mat4( 1.0f );
    glm::mat4 m_projection = glm::mat4( 1.0f );
    glm::mat4 m_viewInverse = glm::mat4( 1.0f );
    glm::mat4 m_projectionInverse = glm::mat4( 1.0f );
  };

  struct LightNodeUbos
  {
    std::vector<std::shared_ptr<DirectionalLightNode>> m_directionalLightNodes;
    std::vector<std::shared_ptr<PointLightNode>> m_pointLightNodes;

    // void add( std::shared_ptr<DirectionalLightNode> directionalLightNodes ) { }
    // void add( std::shared_ptr<PointLightNode> pointLightNodes ) { }
  };

  /// A specialised buffer for uniforms.
  /// @ingroup API
  class UniformBuffer
  {
  public:
    UniformBuffer( ) = default;
    
    /// @param swapchainImagesCount The amount of images in the swapchain.
    /// @param initialize If true, the uniform buffer will be initialized right away without an additional call to init().
    template <typename T>
    UniformBuffer( size_t swapchainImagesCount, bool initialize = true )
    {
      if ( initialize )
        init<T>( swapchainImagesCount );
    }

    /// @return Returns the vector of uniform buffers.
    inline const std::vector<Buffer>& get( ) const { return m_buffers; }

    /// @return Returns the vector of uniform buffers as raw Vulkan buffer objects.
    RX_API const std::vector<vk::Buffer> getRaw( ) const;

    /// Creates the uniform buffer and allocates memory for it.
    /// 
    /// The function will create as many uniform buffers as there are images in the swapchain.
    /// @param swapchainImagesCount The amount of images in the swapchain.
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

    /// Used to fill an image's buffer.
    /// @param imageIndex The image's index in the swapchain.
    /// @param ubo The actual uniform buffer object holding the data.
    template <typename T>
    void upload( uint32_t imageIndex, T& ubo )
    {
      m_buffers[imageIndex].fill<T>( &ubo );
    }

  private:
    std::vector<Buffer> m_buffers; ///< A vector of rx::Buffers for the uniform buffers.
  };

}

#endif // UNIFORM_BUFFER_HPP