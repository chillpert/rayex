#include "UniformBuffer.hpp"
#include "Components.hpp"

namespace rx
{
  UniformBuffer::UniformBuffer( size_t swapchainImagesCount, bool initialize )
  {
    if ( initialize )
      init( swapchainImagesCount );
  }

  const std::vector<vk::Buffer> UniformBuffer::getRaw( ) const
  {
    std::vector<vk::Buffer> res( m_buffers.size( ), nullptr );

    for ( size_t i = 0; i < m_buffers.size( ); ++i )
      res[i] = m_buffers[i].get( );

    return res;
  }

  void UniformBuffer::init( size_t swapchainImagesCount )
  {
    m_buffers.resize( swapchainImagesCount );

    for ( Buffer& buffer : m_buffers )
    {
      buffer.init( sizeof( UniformBufferObject ),
                   vk::BufferUsageFlagBits::eUniformBuffer,
                   { },
                   vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
    }
  }

  void UniformBuffer::upload( uint32_t imageIndex, UniformBufferObject& ubo )
  {
    m_buffers[imageIndex].fill<UniformBufferObject>( &ubo );
  }
}