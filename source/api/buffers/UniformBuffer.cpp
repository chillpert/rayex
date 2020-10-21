#include "api/buffers/UniformBuffer.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  auto UniformBuffer::getRaw( ) const -> const std::vector<vk::Buffer>
  {
    std::vector<vk::Buffer> res( _buffers.size( ), nullptr );

    for ( size_t i = 0; i < _buffers.size( ); ++i )
    {
      res[i] = _buffers[i].get( );
    }

    return res;
  }
} // namespace RAYEX_NAMESPACE
