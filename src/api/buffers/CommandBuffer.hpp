#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  class CommandBuffer
  {
  public:
    CommandBuffer( ) = default;
    CommandBuffer( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit, bool initialize = true );

    void init( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

    inline const std::vector<vk::CommandBuffer> get( ) const { return m_commandBuffers; }
    inline const vk::CommandBuffer get( size_t index ) const { return m_commandBuffers[index]; }

    void free( );
    RX_API void reset( );

    void begin( size_t index = 0 );
    void end( size_t index = 0 );

    void submitToQueue( vk::Queue queue, const std::vector<vk::Semaphore>& waitSemaphores = { }, const std::vector<vk::Semaphore>& signalSemaphores = { }, vk::PipelineStageFlags* waitDstStageMask = { } );

  private:
    std::vector<vk::CommandBuffer> m_commandBuffers;
    
    vk::CommandPool m_commandPool;
    vk::CommandBufferBeginInfo m_beginInfo;
  };
}

#endif // COMMAND_BUFFER_HPP