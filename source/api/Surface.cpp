#include "api/Surface.hpp"

#include "api/Components.hpp"
#include "api/utility/Util.hpp"

namespace RAYEX_NAMESPACE
{
  Surface::~Surface( )
  {
    destroy( );
  }

  void Surface::init( std::shared_ptr<Window> window )
  {
    VkSurfaceKHR surface;
    SDL_bool result = SDL_Vulkan_CreateSurface( window->get( ), components::instance, &surface );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to create surface" );
    }

    _surface            = surface;
    components::surface = _surface;
    RX_ASSERT( _surface, "Failed to create surface." );

    _extent = window->getExtent( );
  }

  void Surface::assessSettings( )
  {
    // Get all surface capabilities.
    _capabilities = components::physicalDevice.getSurfaceCapabilitiesKHR( _surface );

    // Check a present mode.
    std::vector<vk::PresentModeKHR> presentModes = components::physicalDevice.getSurfacePresentModesKHR( _surface );

    if ( !Util::find<vk::PresentModeKHR>( _presentMode, presentModes ) )
    {
      if ( Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eMailbox, presentModes ) )
      {
        _presentMode = vk::PresentModeKHR::eMailbox;
      }
      else if ( Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eImmediate, presentModes ) )
      {
        _presentMode = vk::PresentModeKHR::eImmediate;
      }
      else if ( Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eFifoRelaxed, presentModes ) )
      {
        _presentMode = vk::PresentModeKHR::eFifoRelaxed;
      }
      else
      {
        // Fall back, as FIFO is always supported on every device.
        RX_WARN( "Preferred present mode not available. Falling back to ", vk::to_string( _presentMode ), " present mode." );

        _presentMode = vk::PresentModeKHR::eFifo;
      }
    }

    // Check format and color space.
    auto surfaceFormats = components::physicalDevice.getSurfaceFormatsKHR( _surface );

    bool colorSpaceAndFormatSupported = false;
    for ( const auto& iter : surfaceFormats )
    {
      if ( iter.format == _format && iter.colorSpace == _colorSpace )
      {
        colorSpaceAndFormatSupported = true;
        break;
      }
    }

    // If the prefered format and color space are not available, fall back.
    if ( !colorSpaceAndFormatSupported )
    {
      _format     = surfaceFormats[0].format;
      _colorSpace = surfaceFormats[0].colorSpace;
      RX_WARN( "Preferred format and colorspace not supported. Falling back to the first option of each." );
    }
  }

  void Surface::destroy( )
  {
    if ( _surface )
    {
      components::instance.destroySurfaceKHR( _surface );
      _surface = nullptr;
    }
  }
} // namespace RAYEX_NAMESPACE
