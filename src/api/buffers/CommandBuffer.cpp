#include "CommandBuffer.hpp"
#include "Components.hpp"

namespace RENDERER_NAMESPACE
{
  CommandBuffer::CommandBuffer( vk::CommandPool commandPool, uint32_t count, vk::CommandBufferUsageFlags usageFlags, bool initialize )
  {
    if ( initialize )
      init( commandPool, count, usageFlags );
  }

  void CommandBuffer::init( vk::CommandPool commandPool, uint32_t count, vk::CommandBufferUsageFlags usageFlags )
  {
    m_commandPool = commandPool;

    m_commandBuffers.resize( count );

    vk::CommandBufferAllocateInfo allocateInfo( commandPool,                      // commandPool
                                                vk::CommandBufferLevel::ePrimary, // level
                                                count );                          // commandBufferCount

    m_commandBuffers = g_device.allocateCommandBuffers( allocateInfo );
    for ( const vk::CommandBuffer& commandBuffer : m_commandBuffers )
      RX_ASSERT( commandBuffer, "Failed to create command buffer." );

    // Set up begin info.
    m_beginInfo.flags = usageFlags;
  }

  void CommandBuffer::free( )
  {
    g_device.freeCommandBuffers( m_commandPool, static_cast< uint32_t >( m_commandBuffers.size( ) ), m_commandBuffers.data( ) );
  }

  void CommandBuffer::reset( )
  {
    for ( vk::CommandBuffer& buffer : m_commandBuffers )
      buffer.reset( vk::CommandBufferResetFlagBits::eReleaseResources );
  }

  void CommandBuffer::begin( size_t index )
  {
    m_commandBuffers[index].begin( m_beginInfo );
  }

  void CommandBuffer::end( size_t index )
  {
    m_commandBuffers[index].end( );
  }

  void CommandBuffer::submitToQueue( vk::Queue queue, const std::vector<vk::Semaphore>& waitSemaphores, const std::vector<vk::Semaphore>& signalSemaphores, vk::PipelineStageFlags* waitDstStageMask )
  {
    if ( m_beginInfo.flags & vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
    {
      vk::SubmitInfo submitInfo( static_cast< uint32_t >( waitSemaphores.size( ) ),   // waitSemaphoreCount
                                 waitSemaphores.data( ),                              // pWaitSemaphores
                                 waitDstStageMask,                                    // pWaitDstStageMask
                                 static_cast< uint32_t >( m_commandBuffers.size( ) ), // commandBufferCount
                                 m_commandBuffers.data( ),                            // pCommandBuffers
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