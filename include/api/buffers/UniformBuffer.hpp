#pragma once

#include "api/buffers/Buffer.hpp"
#include "base/Geometry.hpp"

namespace RAYEX_NAMESPACE
{
  /// A memory aligned uniform buffer object for camera data.
  /// @ingroup API
  struct CameraUbo
  {
    glm::mat4 view              = glm::mat4( 1.0F );
    glm::mat4 projection        = glm::mat4( 1.0F );
    glm::mat4 viewInverse       = glm::mat4( 1.0F );
    glm::mat4 projectionInverse = glm::mat4( 1.0F );
    glm::vec4 position          = glm::vec4( 1.0F );

  private:
    glm::vec4 padding0 = glm::vec4( 1.0F ); ///< Padding (ignore).
    glm::vec4 padding1 = glm::vec4( 1.0F ); ///< Padding (ignore).
    glm::vec4 padding2 = glm::vec4( 1.0F ); ///< Padding (ignore).
  };

  /// A uniform buffer specialization class.
  /// @ingroup API
  template <class T>
  class UniformBuffer
  {
  public:
    UniformBuffer( ) = default;

    auto get( ) const -> const std::vector<Buffer>& { return _buffers; }

    /// Creates the uniform buffer and allocates memory for it.
    ///
    /// The function will create as many uniform buffers as there are images in the swapchain.
    /// Additionally, it will create the descriptor buffer infos which can be later used to write to a descriptor set.
    void init( )
    {
      _buffers.resize( components::maxResources );

      for ( Buffer& buffer : _buffers )
      {
        buffer.init( sizeof( T ),
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     { },
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
      }

      _bufferInfos.resize( components::maxResources );

      for ( size_t i = 0; i < _buffers.size( ); ++i )
      {
        _bufferInfos[i] = vk::DescriptorBufferInfo( _buffers[i].get( ), 0, sizeof( T ) );
      }
    }

    /// Used to fill an image's buffer.
    /// @param imageIndex The image's index in the swapchain.
    /// @param ubo The actual uniform buffer object holding the data.
    void upload( uint32_t imageIndex, T& ubo )
    {
      _buffers[imageIndex].fill<T>( &ubo );
    }

    std::vector<vk::DescriptorBufferInfo> _bufferInfos;

  private:
    std::vector<Buffer> _buffers; ///< Holds the uniform buffer and all its copies.
  };
} // namespace RAYEX_NAMESPACE
