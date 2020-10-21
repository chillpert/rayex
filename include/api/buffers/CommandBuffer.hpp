#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for Vulkan command buffers.
  /// @ingroup API
  class CommandBuffer
  {
  public:
    CommandBuffer( ) = default;

    /// Creates the command buffers and calls initializes them right away.
    /// @param commandPool The command pool from which the command buffers will be allocated from.
    /// @param count The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain).
    /// @param usageFlags Specifies what the buffer will be used for.
    CommandBuffer( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

    /// Creates the command buffers.
    /// @param commandPool The command pool from which the command buffers will be allocated from.
    /// @param count The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain).
    /// @param usageFlags Specifies what the buffer will be used for.
    void init( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

    /// @return Returns the vector of command buffers.
    auto get( ) const -> const std::vector<vk::CommandBuffer> { return _commandBuffers; }

    /// Returns the command buffer by some index.
    /// @param index The index of the desired command buffer.
    /// @return Returns the command buffer.
    auto get( size_t index ) const -> const vk::CommandBuffer { return _commandBuffers[index]; }

    /// Frees the command buffer.
    void free( );

    /// Resets the command buffer.
    void reset( );

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
    /// @param waitDstStageMask The pipeline stage where the commands will be executed.
    void submitToQueue( vk::Queue queue, const std::vector<vk::Semaphore>& waitSemaphores = { }, const std::vector<vk::Semaphore>& signalSemaphores = { }, vk::PipelineStageFlags* waitDstStageMask = { } );

  private:
    std::vector<vk::CommandBuffer> _commandBuffers; ///< A vector of Vulkan command buffers.

    vk::CommandPool _commandPool;          ///< The Vulkan command pool used to allocate the command buffer from.
    vk::CommandBufferBeginInfo _beginInfo; ///< The Vulkan begin information of the command buffer.
  };
} // namespace RAYEX_NAMESPACE

#endif // COMMAND_BUFFER_HPP
