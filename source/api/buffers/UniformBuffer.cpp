#include "api/buffers/UniformBuffer.hpp"

#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  auto UniformBuffer::getRaw( ) const -> const std::vector<vk::Buffer>
  {
    std::vector<vk::Buffer> res( this->buffers.size( ), nullptr );

    for ( size_t i = 0; i < this->buffers.size( ); ++i )
    {
      res[i] = this->buffers[i].get( );
    }

    /*
    std::vector<vk::Buffer> res;
    res.reserve( this->buffers.size( ) );

    for ( const auto& buffer : this->buffers )
      res.push_back( buffer.get( ) );
    */

    return res;
  }
} // namespace RAYEXEC_NAMESPACE