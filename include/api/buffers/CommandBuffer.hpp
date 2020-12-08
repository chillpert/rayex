#pragma once

#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for Vulkan command buffer objects.
  /// @ingroup API
  class CommandBuffer
  {
  public:
    CommandBuffer( ) = default;

    /// Call to init(vk::CommandPool, uint32_t, vk::CommandBufferUsageFlags).
    CommandBuffer( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

    /// Creates the command buffers.
    /// @param commandPool The command pool from which the command buffers will be allocated from.
    /// @param count The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain).
    /// @param usageFlags Specifies what the buffer will be used for.
    void init( vk::CommandPool commandPool, uint32_t count = 1, vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

    auto get( ) const -> const std::vector<vk::CommandBuffer> { return _commandBuffers; }

    auto get( size_t index ) const -> const vk::CommandBuffer { return _commandBuffers[index]; }

    void free( );

    void reset( );

    /// Used to begin the command buffer recording.
    /// @param index An index to a command buffer to record to.
    void begin( size_t index = 0 );

    /// Used to stop the command buffer recording.
    /// @param index An index to a command buffer to stop recording.
    void end( size_t index = 0 );

    /// Submits the recorded commands to a queue.
    /// @param queue The queue to submit to.
    /// @param waitSemaphores A std::vector of semaphores to wait for.
    /// @param signalSemaphores A std::vector of semaphores to signal.
    /// @param waitDstStageMask The pipeline stage where the commands will be executed.
    void submitToQueue( vk::Queue queue, vk::Fence fence = nullptr, const std::vector<vk::Semaphore>& waitSemaphores = { }, const std::vector<vk::Semaphore>& signalSemaphores = { }, vk::PipelineStageFlags* waitDstStageMask = { } );

  private:
    std::vector<vk::CommandBuffer> _commandBuffers;

    vk::CommandPool _commandPool; ///< The command pool used to allocate the command buffer from.
    vk::CommandBufferBeginInfo _beginInfo;
  };
} // namespace RAYEX_NAMESPACE
