#pragma once

#include "api/Components.hpp"
#include "api/image/Image.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for a Vulkan buffer object.
  /// @ingroup API
  class Buffer
  {
  public:
    Buffer( ) = default;

    /// Call to init(k::DeviceSize, vk::BufferUsageFlags, const std::vector<uint32_t>&, vk::MemoryPropertyFlags).
    Buffer( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices = { }, vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal, void* pNextMemory = nullptr );

    /// If memory was mapped previously, the destructor will automatically unmap it again.
    ~Buffer( );

    /// @param buffer The target for the copy operation.
    Buffer( const Buffer& buffer );
    Buffer( const Buffer&& ) = delete;

    /// Call to copyToBuffer(Buffer).
    auto operator=( const Buffer& ) -> Buffer&;
    auto operator=( const Buffer && ) -> Buffer& = delete;

    auto get( ) const -> const vk::Buffer { return _buffer.get( ); }

    auto getMemory( ) const -> const vk::DeviceMemory { return _memory.get( ); }

    auto getSize( ) const -> const vk::DeviceSize { return _size; }

    /// Creates the buffer and allocates memory for it.
    /// @param queueFamilyIndices Specifies which queue family will access the buffer.
    /// @param memoryPropertyFlags Flags for memory allocation.
    /// @param pNextMemory Attachment to the memory's pNext chain.
    void init( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices = { }, vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal, void* pNextMemory = nullptr );

    /// Copies the content of this buffer to another RAYEX_NAMESPACE::Buffer.
    /// @param buffer The target for the copy operation.
    /// @param fence A fence to wait for when submitting the local single-time-use command buffer to the command queue.
    void copyToBuffer( const Buffer& buffer, vk::Fence fence = nullptr ) const;

    /// Copies the content of this buffer to another vk::Buffer.
    /// @param buffer The target for the copy operation.
    /// @param fence A fence to wait for when submitting the local single-time-use command buffer to the command queue.
    void copyToBuffer( vk::Buffer buffer, vk::Fence fence = nullptr ) const;

    /// Copies the content of this buffer to an image.
    /// @param image The target for the copy operation.
    void copyToImage( const Image& image ) const;

    /// Copies the content of this buffer to an image.
    /// @param image The target for the copy operation.
    /// @param extent The target's extent.
    void copyToImage( vk::Image image, vk::Extent3D extent ) const;

    /// Used to fill the buffer with the content of a given std::vector.
    /// @param data The data to fill the buffer with.
    /// @param offset The data's offset within the buffer.
    template <class T>
    void fill( const std::vector<T>& data, vk::DeviceSize offset = 0, std::optional<vk::DeviceSize> size = { } )
    {
      vk::DeviceSize actualSize = data.size( ) * sizeof( data[0] );

      // Only call mapMemory once or every time the buffer has been initialized again.
      if ( !_mapped )
      {
        _mapped = true;

        if ( components::device.mapMemory( _memory.get( ), offset, actualSize, { }, &_ptrToData ) != vk::Result::eSuccess )
        {
          RX_ERROR( "Failed to map memory." );
        }
      }

      RX_ASSERT( _ptrToData != nullptr, "Failed to copy data to storage staging buffer." );
      memcpy( _ptrToData, data.data( ), static_cast<uint32_t>( actualSize ) );
    }

    /// Used to fill the buffer by using a pointer and (optionally) passing the underlying memory size.
    /// @param data The data to fill the buffer with.
    /// @param offset The data's offset within the buffer.
    /// @param size An optional size parameter to pass the size of data. If omitted, the size passed when calling init() will be used instead.
    template <class T>
    void fill( const T* data, vk::DeviceSize offset = 0, std::optional<vk::DeviceSize> size = { } )
    {
      vk::DeviceSize finalSize = _size;
      if ( size.has_value( ) )
      {
        finalSize = size.value( );
      }

      // Only call mapMemory once or every time the buffer has been initialized again.
      if ( !_mapped )
      {
        _mapped = true;

        if ( components::device.mapMemory( _memory.get( ), offset, finalSize, { }, &_ptrToData ) != vk::Result::eSuccess )
        {
          RX_ERROR( "Failed to map memory." );
        }
      }

      RX_ASSERT( _ptrToData != nullptr, "Failed to copy data to storage staging buffer." );
      memcpy( _ptrToData, data, static_cast<uint32_t>( _size ) );
      // @todo unmap memory in destructor
    }

  protected:
    vk::UniqueBuffer _buffer;
    vk::UniqueDeviceMemory _memory;

    vk::DeviceSize _size = 0;

    void* _ptrToData = nullptr;
    bool _mapped     = false;
  };
} // namespace RAYEX_NAMESPACE
