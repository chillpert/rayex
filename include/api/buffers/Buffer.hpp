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
    auto operator=( const Buffer&& ) -> Buffer& = delete;

    virtual ~Buffer( ) = default;

    /// Copies the content of this buffer to another RAYEX_NAMESPACE::Buffer.
    /// @param buffer The target for the copy operation.
    void copyToBuffer( const Buffer& buffer ) const;

    /// Copies the content of this buffer to another vk::Buffer.
    /// @param buffer The target for the copy operation.
    void copyToBuffer( vk::Buffer buffer, vk::Fence fence = nullptr ) const;

    /// Copies the content of this buffer to an image.
    /// @param image The target for the copy operation.
    /// @param extent The target's extent.
    void copyToImage( vk::Image image, vk::Extent3D extent ) const;

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
    void fill( const std::vector<T>& data, vk::DeviceSize offset = 0, std::optional<vk::DeviceSize> size = { } )
    {
      vk::DeviceSize actualSize = data.size( ) * sizeof( data[0] );

      // Only call mapMemory once or everytime the buffer has been initialized again.
      if ( !_mapped )
      {
        _mapped = true;

        RX_ASSERT( offset == 0, "DEBUG ASSERT OFFSET == 0" );

        if ( components::device.mapMemory( _memory.get( ), offset, actualSize, { }, &_ptrToData ) != vk::Result::eSuccess )
        {
          RX_ERROR( "Failed to map memory." );
        }
      }

      RX_ASSERT( _ptrToData != nullptr, "Failed to copy data to storage staging buffer." );
      memcpy( _ptrToData, data.data( ), static_cast<uint32_t>( actualSize ) );
      // @todo unmap memory in destructor
    }

    template <class T>
    void fill( const T* source, vk::DeviceSize offset = 0, std::optional<vk::DeviceSize> size = { } )
    {
      vk::DeviceSize finalSize = _size;
      if ( size.has_value( ) )
      {
        finalSize = size.value( );
      }

      // Only call mapMemory once or everytime the buffer has been initialized again.
      if ( !_mapped )
      {
        _mapped = true;

        if ( components::device.mapMemory( _memory.get( ), offset, finalSize, { }, &_ptrToData ) != vk::Result::eSuccess )
        {
          RX_ERROR( "Failed to map memory." );
        }
      }

      RX_ASSERT( _ptrToData != nullptr, "Failed to copy data to storage staging buffer." );
      memcpy( _ptrToData, source, static_cast<uint32_t>( _size ) );
      // @todo unmap memory in destructor
    }

  protected:
    vk::UniqueBuffer _buffer;       ///< The buffer object with a unique handle.
    vk::UniqueDeviceMemory _memory; ///< The buffer's memory with a unique handle.

    vk::DeviceSize _size = 0; ///< The buffer's size.

    void* _ptrToData = nullptr;
    bool _mapped     = false;
  };
} // namespace RAYEX_NAMESPACE

#endif // BUFFER_HPP
