#pragma once

#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  class Sync
  {
  public:
    auto getMaxFramesInFlight( ) -> size_t { return _maxFramesInFlight; }

    auto getImageInFlight( size_t imageIndex ) -> vk::Fence { return _imagesInFlight[imageIndex]; }

    auto getInFlightFence( size_t imageIndex ) -> vk::Fence { return _inFlightFences[imageIndex].get( ); }

    auto getImageAvailableSemaphore( size_t imageIndex ) -> vk::Semaphore { return _imageAvailableSemaphores[imageIndex].get( ); }

    auto getFinishedRenderSemaphore( size_t imageIndex ) -> vk::Semaphore { return _finishedRenderSemaphores[imageIndex].get( ); }

    void init( );

    void waitForFrame( size_t frame );

  private:
    std::vector<vk::Fence> _imagesInFlight;
    std::vector<vk::UniqueFence> _inFlightFences;
    std::vector<vk::UniqueSemaphore> _imageAvailableSemaphores;
    std::vector<vk::UniqueSemaphore> _finishedRenderSemaphores;

    // Defines the maximum amount of frames that will be processed concurrently.
    const size_t _maxFramesInFlight = 2;
  };
} // namespace RAYEX_NAMESPACE
