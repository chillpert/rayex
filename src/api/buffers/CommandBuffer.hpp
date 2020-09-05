#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  /// A wrapper class for Vulkan command buffers.
  /// @ingroup API
  class CommandBuffer
  {
  public:
    CommandBuffer( ) = default;
    
    /// @param commandPool The command pool from which the command buffers will be allocated from.
    /// @param count The amount of Vulkan command buffers to initialize.
    /// @param usageFlags Specifies what the buffer will be used for.
    /// @param initialize If true, the command buffers will be initialized right away without an additional call to init().
    CommandBuffer( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit, bool initialize = true );

    /// Creates the command buffers.
    /// @param commandPool The command pool from which the command buffers will be allocated from.
    /// @param count The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain).
    /// @param usageFlags Specifies what the buffer will be used for.
    void init( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

    /// @return Returns the vector of command buffers.
    inline const std::vector<vk::CommandBuffer> get( ) const { return m_commandBuffers; }

    /// Returns the command buffer by some index.
    /// @param index The index of the desired command buffer.
    /// @return Returns the command buffer.
    inline const vk::CommandBuffer get( size_t index ) const
    {
      RX_ASSERT( ( index <= m_commandBuffers.size( ) - 1 ), "Failed to get command buffer because vector is out of range." );
      return m_commandBuffers[index];
    }

    /// Frees the command buffer.
    void free( );
    
    /// Resets the command buffer.
    RX_API void reset( );

    /// Used to begin the command buffer recording.
    /// @param index An index to a command buffer to record to.
    void begin( size_t index = 0 );
    
    /// Used to stop the command buffer recording.
    /// @param index An index to a command buffer to stop recording.
    void end( size_t index = 0 );

    /// Submits the recorded commands to a queue.
    /// @param queue The queue to submit to.
    /// @param waitSemaphores A vector of semaphores to wait for.
    /// @param signalSemaphores A vector of semaphores to signal.
    /// @waitDstStageMask The pipeline stage where the commands will be executed.
    void submitToQueue( vk::Queue queue, const std::vector<vk::Semaphore>& waitSemaphores = { }, const std::vector<vk::Semaphore>& signalSemaphores = { }, vk::PipelineStageFlags* waitDstStageMask = { } );

  private:
    std::vector<vk::CommandBuffer> m_commandBuffers; ///< A vector of Vulkan command buffers.
    
    vk::CommandPool m_commandPool; ///< The Vulkan command pool used to allocate the command buffer from.
    vk::CommandBufferBeginInfo m_beginInfo; ///< The Vulkan begin information of the command buffer.
  };
}

#endif // COMMAND_BUFFER_HPP