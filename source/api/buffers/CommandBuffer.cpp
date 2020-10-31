#include "api/buffers/CommandBuffer.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  CommandBuffer::CommandBuffer( vk::CommandPool commandPool, uint32_t count, vk::CommandBufferUsageFlags usageFlags )
  {
    init( commandPool, count, usageFlags );
  }

  void CommandBuffer::init( vk::CommandPool commandPool, uint32_t count, vk::CommandBufferUsageFlags usageFlags )
  {
    _commandPool = commandPool;

    _commandBuffers.resize( count );

    vk::CommandBufferAllocateInfo allocateInfo( commandPool,                      // commandPool
                                                vk::CommandBufferLevel::ePrimary, // level
                                                count );                          // commandBufferCount

    _commandBuffers = components::device.allocateCommandBuffers( allocateInfo );
    for ( const vk::CommandBuffer& commandBuffer : _commandBuffers )
    {
      RX_ASSERT( commandBuffer, "Failed to create command buffers." );
    }

    // Set up begin info.
    _beginInfo.flags = usageFlags;
  }

  void CommandBuffer::free( )
  {
    components::device.freeCommandBuffers( _commandPool, static_cast<uint32_t>( _commandBuffers.size( ) ), _commandBuffers.data( ) );
  }

  void CommandBuffer::reset( )
  {
    for ( vk::CommandBuffer& buffer : _commandBuffers )
    {
      buffer.reset( vk::CommandBufferResetFlagBits::eReleaseResources );
    }
  }

  void CommandBuffer::begin( size_t index )
  {
    _commandBuffers[index].begin( _beginInfo );
  }

  void CommandBuffer::end( size_t index )
  {
    _commandBuffers[index].end( );
  }

  void CommandBuffer::submitToQueue( vk::Queue queue, vk::Fence fence, const std::vector<vk::Semaphore>& waitSemaphores, const std::vector<vk::Semaphore>& signalSemaphores, vk::PipelineStageFlags* waitDstStageMask )
  {
    if ( _beginInfo.flags & vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
    {
      vk::SubmitInfo submitInfo( static_cast<uint32_t>( waitSemaphores.size( ) ),   // waitSemaphoreCount
                                 waitSemaphores.data( ),                            // pWaitSemaphores
                                 waitDstStageMask,                                  // pWaitDstStageMask
                                 static_cast<uint32_t>( _commandBuffers.size( ) ),  // commandBufferCount
                                 _commandBuffers.data( ),                           // pCommandBuffers
                                 static_cast<uint32_t>( signalSemaphores.size( ) ), // signalSemaphoreCount
                                 signalSemaphores.data( ) );                        // pSignalSemaphores

      if ( queue.submit( 1, &submitInfo, fence ) != vk::Result::eSuccess )
      {
        RX_ERROR( "Failed to submit" );
      }

      queue.waitIdle( );
    }
    else
    {
      RX_ERROR( "Only command buffers with a usage flag containing eOneTimeSubmit should be submitted automatically" );
    }
  }
} // namespace RAYEX_NAMESPACE
