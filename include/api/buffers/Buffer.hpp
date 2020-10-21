#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "api/Components.hpp"
#include "api/image/Image.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for a Vulkan buffer.
  /// @ingroup API
  class Buffer
  {
  public:
    Buffer( ) = default;

    /// @param size The size of the buffer.
    /// @param usage The buffer's usage flags.
    /// @param queueFamilyIndices Specifies which queue family will access the buffer.
    /// @param memoryPropertyFlags Flags for memory allocation.
    /// @param pNextMemory Attachment to the memory's pNext chain.
    /// @param initialize If true, the buffer object will be initialized right away without an additional call to init().
    Buffer( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices = { }, vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal, void* pNextMemory = nullptr, bool initialize = true );

    /// @param buffer The target for the copy operation.
    Buffer( const Buffer& buffer );
    Buffer( const Buffer&& ) = delete;

    auto operator=( const Buffer& ) -> Buffer&;
    auto operator=( const Buffer && ) -> Buffer& = delete;

    virtual ~Buffer( ) = default;

    /// Copies the content of this buffer to another RAYEX_NAMESPACE::Buffer.
    /// @param buffer The target for the copy operation.
    void copyToBuffer( const Buffer& buffer ) const;

    /// Copies the content of this buffer to another vk::Buffer.
    /// @param buffer The target for the copy operation.
    void copyToBuffer( vk::Buffer buffer ) const;

    /// Copies the content of this buffer to an image.
    /// @param image The target for the copy operation.
    void copyToImage( Image& image ) const;

    /// @return Returns the buffer without the unique handle.
    auto get( ) const -> const vk::Buffer { return _buffer.get( ); }

    /// @return Returns the buffer's memory without the unique handle.
    auto getMemory( ) const -> const vk::DeviceMemory { return _memory.get( ); }

    /// @return Returns the size of the buffer.
    auto getSize( ) const -> const vk::DeviceSize { return _size; }

    /// Creates the buffer and allocates memory for it.
    /// @param size The size of the buffer.
    /// @param usage The buffer's usage flags.
    /// @param queueFamilyIndices Specifies which queue family will access the buffer.
    /// @param memoryPropertyFlags Flags for memory allocation.
    /// @param pNextMemory Attachment to the memory's pNext chain.
    void init( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices = { }, vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal, void* pNextMemory = nullptr );

    /// Used to fill the buffer with any kind of data.
    /// @param source The data to fill the buffer with.
    /// @param offset The data's offset within the buffer.
    template <class T>
    void fill( const T* source, vk::DeviceSize offset = 0 )
    {
      void* data;
      if ( components::device.mapMemory( _memory.get( ), offset, _size, { }, &data ) != vk::Result::eSuccess )
        RX_ERROR( "Failed to map memory." );

      memcpy( data, source, static_cast<uint32_t>( _size ) );

      components::device.unmapMemory( _memory.get( ) );
    }

  protected:
    vk::UniqueBuffer _buffer;       ///< The buffer object with a unique handle.
    vk::UniqueDeviceMemory _memory; ///< The buffer's memory with a unique handle.

    vk::DeviceSize _size = 0; ///< The buffer's size.
  };

  template <typename T>
  void createStorageBufferWithStaging( Buffer& buffer, std::vector<T>& data )
  {
    vk::DeviceSize size = sizeof( data[0] ) * data.size( );
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    // Set up the staging buffer.
    Buffer stagingBuffer( size,                                                                                  // size
                          vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { components::transferFamilyIndex },                                                   // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,  // memoryPropertyFlags
                          &allocateFlags );                                                                      // pNext of memory

    buffer.init( size,                                                                                                                                                                     // size
                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eStorageBuffer, // usage
                 { components::transferFamilyIndex },                                                                                                                                      // queueFamilyIndices
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,                                                                                     // memoryPropertyFlags
                 &allocateFlags );                                                                                                                                                         // pNext of memory

    stagingBuffer.fill<T>( data.data( ) );

    // Copy staging buffer to the actual index buffer.
    stagingBuffer.copyToBuffer( buffer.get( ) );
  }
} // namespace RAYEX_NAMESPACE

#endif // BUFFER_HPP
