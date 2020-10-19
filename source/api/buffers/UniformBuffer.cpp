#include "api/buffers/UniformBuffer.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
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
} // namespace RAYEX_NAMESPACE
