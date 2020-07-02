#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Buffer.hpp"
#include "Vertex.hpp"

namespace rx
{
  class VertexBuffer
  {
  public:
    VertexBuffer( ) = default;
    VertexBuffer( std::vector<Vertex>& vertices, bool initialize = true );

    inline const vk::Buffer get( ) const { return m_buffer.get( ); }
    inline vk::DeviceSize getSize( ) const { return m_buffer.getSize( ); }
    inline uint32_t getCount( ) const { return m_count; }

    void init( std::vector<Vertex>& vertices );

  private:
    Buffer m_buffer;

    uint32_t m_count;
  };
}

#endif // VERTEX_BUFFER_HPP