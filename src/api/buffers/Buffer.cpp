#include "Buffer.hpp"
#include "CommandBuffer.hpp"
#include "Helpers.hpp"
#include "Initializers.hpp"

namespace rx
{
  Buffer::Buffer( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices, vk::MemoryPropertyFlags memoryPropertyFlags, void* pNextMemory, bool initialize )
  {
    if ( initialize )
      init( size, usage, queueFamilyIndices, memoryPropertyFlags, pNextMemory );
  }

  Buffer::Buffer( Buffer& buffer )
  {
    copyToBuffer( buffer );
  }

  void Buffer::init( vk::DeviceSize size, vk::BufferUsageFlags usage, const std::vector<uint32_t>& queueFamilyIndices, vk::MemoryPropertyFlags memoryPropertyFlags, void* pNextMemory )
  {
    m_size = size;

    vk::SharingMode sharingMode = queueFamilyIndices.size( ) > 1 ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive;

    vk::BufferCreateInfo createInfo( { },                                                   // flags 
                                     size,                                                  // size
                                     usage,                                                 // usage
                                     sharingMode,                                           // sharingMode
                                     static_cast< uint32_t >( queueFamilyIndices.size( ) ), // queueFamilyIndexCount
                                     queueFamilyIndices.data( ) );                          // pQueueFamilyIndices

    m_buffer = g_device.createBufferUnique( createInfo );
    RX_ASSERT( m_buffer, "Failed to create buffer." );

    m_memory = vk::Initializer::allocateMemoryUnique( m_buffer.get( ), memoryPropertyFlags, pNextMemory );
  }

  void Buffer::copyToBuffer( Buffer& buffer ) const
  {
    CommandBuffer commandBuffer( g_transferCmdPool );
    commandBuffer.begin( );
    {
      vk::BufferCopy copyRegion( 0, 0, m_size );
      commandBuffer.get( 0 ).copyBuffer( m_buffer.get( ), buffer.get( ), 1, &copyRegion ); // CMD
    }
    commandBuffer.end( );
    commandBuffer.submitToQueue( g_transferQueue );
  }

  void Buffer::copyToBuffer( vk::Buffer buffer ) const
  {
    CommandBuffer commandBuffer( g_transferCmdPool );
    commandBuffer.begin( );
    {
      vk::BufferCopy copyRegion( 0, 0, m_size );
      commandBuffer.get( 0 ).copyBuffer( m_buffer.get( ), buffer, 1, &copyRegion ); // CMD
    }
    commandBuffer.end( );
    commandBuffer.submitToQueue( g_transferQueue );
  }

  void Buffer::copyToImage( Image& image ) const
  {
    CommandBuffer commandBuffer( g_graphicsCmdPool );
    commandBuffer.begin( );
    {
      vk::BufferImageCopy region( 0,                                            // bufferOffset 
                                  0,                                            // bufferRowLength
                                  0,                                            // bufferImageHeight
                                  { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, // imageSubresource (aspectMask, mipLevel, baseArrayLayer, layerCount)
                                  vk::Offset3D{ 0, 0, 0 },                      // imageOffset
                                  image.getExtent( ) );                         // imageExtent
      
      commandBuffer.get( 0 ).copyBufferToImage( m_buffer.get( ), image.get( ), vk::ImageLayout::eTransferDstOptimal, 1, &region ); // CMD
    }
    commandBuffer.end( );
    commandBuffer.submitToQueue( g_graphicsQueue );
  }
}