#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "Buffer.hpp"

namespace rx
{
  template <typename T = uint32_t>
  class IndexBuffer
  {
  public:
    IndexBuffer( ) = default;
    IndexBuffer( const std::vector<T>& indices, bool initialize = true );

    inline const vk::Buffer get( ) const { return m_buffer.get( ); }
    inline uint32_t getCount( ) const { return m_count; }
    inline vk::IndexType getType( ) const { return m_indexType; }

    void init( std::vector<T>& indices );

  private:
    Buffer m_buffer;

    uint32_t m_count;
    vk::IndexType m_indexType;
  };

  template<typename T>
  inline IndexBuffer<T>::IndexBuffer( const std::vector<T>& indices, bool initialize )
  {
    if ( initialize )
      init( indices, queueIndices );
  }

  template <typename T>
  void IndexBuffer<T>::init( std::vector<T>& indices )
  {
    m_count = static_cast< uint32_t >( indices.size( ) );

    if ( typeid( T ) == typeid( uint8_t ) )
      m_indexType = vk::IndexType::eUint8EXT;

    else if ( typeid( T ) == typeid( uint16_t ) )
      m_indexType = vk::IndexType::eUint16;

    else if ( typeid( T ) == typeid( uint32_t ) )
      m_indexType = vk::IndexType::eUint32;

    else
      RX_ERROR( "Invalid data type for index buffer was specified." );

    vk::DeviceSize size = sizeof( indices[0] ) * indices.size( );
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    // Set up the staging buffer.
    Buffer stagingBuffer( size,
                          vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                          { g_transferFamilyIndex },
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                          &allocateFlags );

    stagingBuffer.fill<T>( indices.data( ) );

    // Set up the actual index buffer.
    m_buffer.init( size,
                   vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                   { g_transferFamilyIndex },
                   vk::MemoryPropertyFlagBits::eDeviceLocal,
                   &allocateFlags );

    // Copy staging buffer to the actual index buffer.
    stagingBuffer.copyToBuffer( m_buffer );
  }
}

#endif // INDEX_BUFFER_HPP