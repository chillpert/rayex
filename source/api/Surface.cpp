#include "api/Surface.hpp"

#include "api/Components.hpp"
#include "api/utility/Util.hpp"

namespace RAYEX_NAMESPACE
{
  Surface::~Surface( )
  {
    destroy( );
  }

  void Surface::init( )
  {
    VkSurfaceKHR surface;
    SDL_bool result = SDL_Vulkan_CreateSurface( g_window->get( ), g_instance, &surface );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to create surface" );
    }

    _surface  = surface;
    g_surface = _surface;
    RX_ASSERT( _surface, "Failed to create surface." );
  }

  void Surface::assessSettings( )
  {
    // Get all surface capabilities.
    _capabilities = g_physicalDevice.getSurfaceCapabilitiesKHR( _surface );

    // Check a present mode.
    std::vector<vk::PresentModeKHR> presentModes = g_physicalDevice.getSurfacePresentModesKHR( _surface );

    if ( !Util::find<vk::PresentModeKHR>( _presentMode, presentModes ) )
    {
      Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eMailbox, presentModes ) ? _presentMode                                                                                    = vk::PresentModeKHR::eMailbox :
                                                                                     Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eFifoRelaxed, presentModes ) ? _presentMode = vk::PresentModeKHR::eFifoRelaxed :
                                                                                                                                                                        _presentMode = vk::PresentModeKHR::eFifo;

      // Fall back, as FIFO is always supported on every device.
      RX_WARN( "Preferred present mode not available. Falling back to ", vk::to_string( _presentMode ), " present mode." );
    }

    // Check format and color space.
    auto surfaceFormats = g_physicalDevice.getSurfaceFormatsKHR( _surface );

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

    g_surfaceFormat = _format;
  }

  void Surface::destroy( )
  {
    if ( _surface )
    {
      g_instance.destroySurfaceKHR( _surface );
      _surface = nullptr;
    }
  }
} // namespace RAYEX_NAMESPACE
