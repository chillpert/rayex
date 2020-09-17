#include "api/buffers/CommandBuffer.hpp"
#include "api/misc/Components.hpp"

namespace RENDERER_NAMESPACE
{
  CommandBuffer::CommandBuffer( vk::CommandPool commandPool, uint32_t count, vk::CommandBufferUsageFlags usageFlags, bool initialize )
  {
    if ( initialize )
      init( commandPool, count, usageFlags );
  }

  void CommandBuffer::init( vk::CommandPool commandPool, uint32_t count, vk::CommandBufferUsageFlags usageFlags )
  {
    this->commandPool = commandPool;

    this->commandBuffers.resize( count );

    vk::CommandBufferAllocateInfo allocateInfo( commandPool,                      // commandPool
                                                vk::CommandBufferLevel::ePrimary, // level
                                                count );                          // commandBufferCount

    this->commandBuffers = g_device.allocateCommandBuffers( allocateInfo );
    for ( const vk::CommandBuffer& commandBuffer : this->commandBuffers )
      RX_ASSERT( commandBuffer, "Failed to create command buffer." );

    // Set up begin info.
    this->beginInfo.flags = usageFlags;
  }

  void CommandBuffer::free( )
  {
    g_device.freeCommandBuffers( this->commandPool, static_cast< uint32_t >( this->commandBuffers.size( ) ), this->commandBuffers.data( ) );
  }

  void CommandBuffer::reset( )
  {
    for ( vk::CommandBuffer& buffer : this->commandBuffers )
      buffer.reset( vk::CommandBufferResetFlagBits::eReleaseResources );
  }

  void CommandBuffer::begin( size_t index )
  {
    this->commandBuffers[index].begin( this->beginInfo );
  }

  void CommandBuffer::end( size_t index )
  {
    this->commandBuffers[index].end( );
  }

  void CommandBuffer::submitToQueue( vk::Queue queue, const std::vector<vk::Semaphore>& waitSemaphores, const std::vector<vk::Semaphore>& signalSemaphores, vk::PipelineStageFlags* waitDstStageMask )
  {
    if ( this->beginInfo.flags & vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
    {
      vk::SubmitInfo submitInfo( static_cast< uint32_t >( waitSemaphores.size( ) ),   // waitSemaphoreCount
                                 waitSemaphores.data( ),                              // pWaitSemaphores
                                 waitDstStageMask,                                    // pWaitDstStageMask
                                 static_cast< uint32_t >( this->commandBuffers.size( ) ), // commandBufferCount
                                 this->commandBuffers.data( ),                            // pCommandBuffers
                                 static_cast< uint32_t >( signalSemaphores.size( ) ), // signalSemaphoreCount
                                 signalSemaphores.data( ) );                          // pSignalSemaphores

      if ( queue.submit( 1, &submitInfo, nullptr ) != vk::Result::eSuccess )
        RX_ERROR( "Failed to submit" );

      queue.waitIdle( );
    }
    else
      RX_ERROR( "Only command buffers with a usage flag containing eOneTimeSubmit should be submitted automatically" );
  }
}