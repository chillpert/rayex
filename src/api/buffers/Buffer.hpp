#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Image.hpp"
#include "Components.hpp"

namespace rx
{
  class Buffer
  {
  public:
    Buffer( ) = default;
    Buffer( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices = { }, vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal, void* pNextMemory = nullptr, bool initialize = true );

    void copyToBuffer( Buffer& buffer ) const;
    void copyToImage( Image& image ) const;

    inline const vk::Buffer get( ) const { return m_buffer.get( ); }
    inline const vk::DeviceMemory getMemory( ) const { return m_memory.get( ); }
    inline const vk::DeviceSize getSize( ) const { return m_size; }

    void init( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices = { }, vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal, void* pNextMemory = nullptr );

    template <class T>
    void fill( T* source, vk::DeviceSize offset = 0 );

  private:
    vk::UniqueBuffer m_buffer;
    vk::UniqueDeviceMemory m_memory;

    vk::DeviceSize m_size = 0;
  };

  template <class T>
  inline void Buffer::fill( T* source, vk::DeviceSize offset )
  {
    void* data;
    if ( g_device.mapMemory( m_memory.get( ), offset, m_size, { }, &data ) != vk::Result::eSuccess )
      RX_ERROR( "Failed to map memory." );

    memcpy( data, source, static_cast< uint32_t >( m_size ) );

    g_device.unmapMemory( m_memory.get( ) );
  }
}

#endif // BUFFER_HPP