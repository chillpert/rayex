#include "api/Sync.hpp"

#include "api/Components.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  void Sync::init( )
  {
    _imageAvailableSemaphores.resize( _maxFramesInFlight );
    _finishedRenderSemaphores.resize( _maxFramesInFlight );
    _inFlightFences.resize( _maxFramesInFlight );
    _imagesInFlight.resize( components::swapchainImageCount, nullptr );

    for ( size_t i = 0; i < _maxFramesInFlight; ++i )
    {
      _imageAvailableSemaphores[i] = vkCore::initSemaphoreUnique( );
      _finishedRenderSemaphores[i] = vkCore::initSemaphoreUnique( );
      _inFlightFences[i]           = vkCore::initFenceUnique( vk::FenceCreateFlagBits::eSignaled );
    }
  }

  void Sync::waitForFrame( size_t frame )
  {
    vk::Result result = components::device.waitForFences( 1, &_inFlightFences[frame].get( ), VK_TRUE, UINT64_MAX );
    RX_ASSERT( result == vk::Result::eSuccess, "Failed to wait for fences." );
  }
} // namespace RAYEX_NAMESPACE
