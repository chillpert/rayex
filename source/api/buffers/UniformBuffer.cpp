#include "UniformBuffer.hpp"
#include "Components.hpp"

namespace RENDERER_NAMESPACE
{
  const std::vector<vk::Buffer> UniformBuffer::getRaw( ) const
  {
    std::vector<vk::Buffer> res( m_buffers.size( ), nullptr );

    for ( size_t i = 0; i < m_buffers.size( ); ++i )
      res[i] = m_buffers[i].get( );

    return res;
  }
}