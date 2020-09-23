#include "api/buffers/UniformBuffer.hpp"
#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  const std::vector<vk::Buffer> UniformBuffer::getRaw( ) const
  {
    std::vector<vk::Buffer> res( this->buffers.size( ), nullptr );

    for ( size_t i = 0; i < this->buffers.size( ); ++i )
      res[i] = this->buffers[i].get( );

    return res;
  }
}