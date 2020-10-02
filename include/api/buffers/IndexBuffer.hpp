#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "api/buffers/Buffer.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A specialised buffer for index data.
  /// @note If not templatized then uint32_t will be used as the data's type.
  /// @ingroup API
  template <typename T = uint32_t>
  class IndexBuffer : public Buffer
  {
  public:
    IndexBuffer( ) = default;

    /// @param The index data.
    /// @param initialize If true, the index buffer object will be initialized right away without an additional call to init().
    IndexBuffer( const std::vector<T>& indices, bool initialize = true )
    {
      if ( initialize )
        init( indices );
    }

    /// @return Returns the amount of indices in the buffer.
    inline uint32_t getCount( ) const { return count; }

    /// @return Returns the type of the index's data.
    /// @note Should be identical to the type that was used to templatize the class.
    inline vk::IndexType getType( ) const { return indexType; }

    /// Creates the buffer, allocates memory for it and fills it with the provided data.
    ///
    /// To optimize the procedure a temporary staging buffer will be created.
    /// @param indices The index data.
    void init( std::vector<T>& indices )
    {
      count = static_cast<uint32_t>( indices.size( ) );

      if ( typeid( T ) == typeid( uint8_t ) )
        indexType = vk::IndexType::eUint8EXT;

      else if ( typeid( T ) == typeid( uint16_t ) )
        indexType = vk::IndexType::eUint16;

      else if ( typeid( T ) == typeid( uint32_t ) )
        indexType = vk::IndexType::eUint32;

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
      Buffer::init( size,
                    vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress,
                    { g_transferFamilyIndex },
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    &allocateFlags );

      // Copy staging buffer to the actual index buffer.
      stagingBuffer.copyToBuffer( buffer.get( ) );
    }

  private:
    uint32_t count;          ///< The amount of indices in the buffer.
    vk::IndexType indexType; ///< The type of the index's data
  };
} // namespace RAYEXEC_NAMESPACE

#endif // INDEX_BUFFER_HPP