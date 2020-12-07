#pragma once

#include "api/Components.hpp"

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

    void init( )
    {
      _imageAvailableSemaphores.resize( _maxFramesInFlight );
      _finishedRenderSemaphores.resize( _maxFramesInFlight );
      _inFlightFences.resize( _maxFramesInFlight );
      _imagesInFlight.resize( components::swapchainImageCount, nullptr );

      for ( size_t i = 0; i < _maxFramesInFlight; ++i )
      {
        _imageAvailableSemaphores[i] = vk::Initializer::initSemaphoreUnique( );
        _finishedRenderSemaphores[i] = vk::Initializer::initSemaphoreUnique( );
        _inFlightFences[i]           = vk::Initializer::initFenceUnique( vk::FenceCreateFlagBits::eSignaled );
      }
    }

    void waitForFrame( size_t frame )
    {
      vk::Result result = components::device.waitForFences( 1, &_inFlightFences[frame].get( ), VK_TRUE, UINT64_MAX );
      RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );
    }

  private:
    std::vector<vk::Fence> _imagesInFlight;
    std::vector<vk::UniqueFence> _inFlightFences;
    std::vector<vk::UniqueSemaphore> _imageAvailableSemaphores;
    std::vector<vk::UniqueSemaphore> _finishedRenderSemaphores;

    // Defines the maximum amount of frames that will be processed concurrently.
    const size_t _maxFramesInFlight = 2;
  };
} // namespace RAYEX_NAMESPACE
