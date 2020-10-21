#include "api/buffers/VertexBuffer.hpp"

namespace RAYEX_NAMESPACE
{
  VertexBuffer::VertexBuffer( std::vector<Vertex>& vertices, bool initialize )
  {
    if ( initialize )
    {
      init( vertices );
    }
  }

  void VertexBuffer::init( const std::vector<Vertex>& vertices )
  {
    _count = static_cast<uint32_t>( vertices.size( ) );

    vk::DeviceSize size = sizeof( vertices[0] ) * vertices.size( );
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    // Set up the staging buffer.
    Buffer stagingBuffer( size,                                                                                  // size
                          vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { components::transferFamilyIndex },                                                   // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,  // memoryPropertyFlags
                          &allocateFlags );                                                                      // pNext of memory

    Buffer::init( size,                                                                                                                                                                     // size
                  vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eStorageBuffer, // usage
                  { components::transferFamilyIndex },                                                                                                                                      // queueFamilyIndices
                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,                                                                                     // memoryPropertyFlags
                  &allocateFlags );                                                                                                                                                         // pNext of memory

    stagingBuffer.fill<Vertex>( vertices.data( ) );

    // Copy staging buffer to the actual index buffer.
    stagingBuffer.copyToBuffer( _buffer.get( ) );
  }
} // namespace RAYEX_NAMESPACE
