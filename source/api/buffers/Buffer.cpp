#include "api/buffers/Buffer.hpp"

#include "api/buffers/CommandBuffer.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  Buffer::Buffer( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices, vk::MemoryPropertyFlags memoryPropertyFlags, void* pNextMemory, bool initialize )
  {
    if ( initialize )
    {
      init( size, usage, queueFamilyIndices, memoryPropertyFlags, pNextMemory );
    }
  }

  Buffer::Buffer( const Buffer& buffer )
  {
    copyToBuffer( buffer );
  }

  auto Buffer::operator=( const Buffer& buffer ) -> Buffer&
  {
    buffer.copyToBuffer( *this );
    return *this;
  }

  void Buffer::init( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices, vk::MemoryPropertyFlags memoryPropertyFlags, void* pNextMemory )
  {
    _mapped = false;

    _size = size;

    vk::SharingMode sharingMode = queueFamilyIndices.size( ) > 1 ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive;

    vk::BufferCreateInfo createInfo( { },                                                 // flags
                                     size,                                                // size
                                     usage,                                               // usage
                                     sharingMode,                                         // sharingMode
                                     static_cast<uint32_t>( queueFamilyIndices.size( ) ), // queueFamilyIndexCount
                                     queueFamilyIndices.data( ) );                        // pQueueFamilyIndices

    _buffer = components::device.createBufferUnique( createInfo );
    RX_ASSERT( _buffer.get( ), "Failed to create buffer." );

    _memory = vk::Initializer::allocateMemoryUnique( _buffer.get( ), memoryPropertyFlags, pNextMemory );
  }

  void Buffer::copyToBuffer( const Buffer& buffer ) const
  {
    CommandBuffer commandBuffer( components::transferCmdPool );
    commandBuffer.begin( );
    {
      vk::BufferCopy copyRegion( 0, 0, _size );
      commandBuffer.get( 0 ).copyBuffer( _buffer.get( ), buffer.get( ), 1, &copyRegion ); // CMD
    }
    commandBuffer.end( );
    commandBuffer.submitToQueue( components::transferQueue );
  }

  void Buffer::copyToBuffer( vk::Buffer buffer, vk::Fence fence ) const
  {
    CommandBuffer commandBuffer( components::transferCmdPool );
    commandBuffer.begin( );
    {
      vk::BufferCopy copyRegion( 0, 0, _size );
      commandBuffer.get( 0 ).copyBuffer( _buffer.get( ), buffer, 1, &copyRegion ); // CMD
    }
    commandBuffer.end( );
    commandBuffer.submitToQueue( components::transferQueue, fence );
  }

  void Buffer::copyToImage( vk::Image image, vk::Extent3D extent ) const
  {
    CommandBuffer commandBuffer( components::graphicsCmdPool );
    commandBuffer.begin( );
    {
      vk::BufferImageCopy region( 0,                                            // bufferOffset
                                  0,                                            // bufferRowLength
                                  0,                                            // bufferImageHeight
                                  { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, // imageSubresource (aspectMask, mipLevel, baseArrayLayer, layerCount)
                                  vk::Offset3D { 0, 0, 0 },                     // imageOffset
                                  extent );                                     // imageExtent

      commandBuffer.get( 0 ).copyBufferToImage( _buffer.get( ), image, vk::ImageLayout::eTransferDstOptimal, 1, &region ); // CMD
    }
    commandBuffer.end( );
    commandBuffer.submitToQueue( components::graphicsQueue );
  }
} // namespace RAYEX_NAMESPACE
