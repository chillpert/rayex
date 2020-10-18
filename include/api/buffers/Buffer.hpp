#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "api/Components.hpp"
#include "api/image/Image.hpp"

namespace RAYEXEC_NAMESPACE
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

    RX_API virtual ~Buffer( ) = default;

    /// Copies the content of this buffer to another RAYEXEC_NAMESPACE::Buffer.
    /// @param buffer The target for the copy operation.
    void copyToBuffer( const Buffer& buffer ) const;

    /// Copies the content of this buffer to another vk::Buffer.
    /// @param buffer The target for the copy operation.
    RX_API void copyToBuffer( vk::Buffer buffer ) const;

    /// Copies the content of this buffer to an image.
    /// @param image The target for the copy operation.
    void copyToImage( Image& image ) const;

    /// @return Returns the buffer without the unique handle.
    [[nodiscard]] auto get( ) const -> const vk::Buffer { return buffer.get( ); }

    /// @return Returns the buffer's memory without the unique handle.
    [[nodiscard]] auto getMemory( ) const -> const vk::DeviceMemory { return memory.get( ); }

    /// @return Returns the size of the buffer.
    [[nodiscard]] auto getSize( ) const -> const vk::DeviceSize { return size; }

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
      if ( g_device.mapMemory( memory.get( ), offset, size, { }, &data ) != vk::Result::eSuccess )
        RX_ERROR( "Failed to map memory." );

      memcpy( data, source, static_cast<uint32_t>( size ) );

      g_device.unmapMemory( memory.get( ) );
    }

  protected:
    vk::UniqueBuffer buffer;       ///< The buffer object with a unique handle.
    vk::UniqueDeviceMemory memory; ///< The buffer's memory with a unique handle.

    vk::DeviceSize size = 0; ///< The buffer's size.
  };

  template <typename T>
  void createStorageBufferWithStaging( Buffer& buffer, std::vector<T>& data )
  {
    vk::DeviceSize size = sizeof( data[0] ) * data.size( );
    vk::MemoryAllocateFlagsInfo allocateFlags( vk::MemoryAllocateFlagBitsKHR::eDeviceAddress );

    // Set up the staging buffer.
    Buffer stagingBuffer( size,                                                                                  // size
                          vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress, // usage
                          { g_transferFamilyIndex },                                                             // queueFamilyIndices
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,  // memoryPropertyFlags
                          &allocateFlags );                                                                      // pNext of memory

    buffer.init( size,                                                                                                                                                                     // size
                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eStorageBuffer, // usage
                 { g_transferFamilyIndex },                                                                                                                                                // queueFamilyIndices
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,                                                                                     // memoryPropertyFlags
                 &allocateFlags );                                                                                                                                                         // pNext of memory

    stagingBuffer.fill<T>( data.data( ) );

    // Copy staging buffer to the actual index buffer.
    stagingBuffer.copyToBuffer( buffer.get( ) );
  }
} // namespace RAYEXEC_NAMESPACE

#endif // BUFFER_HPP
