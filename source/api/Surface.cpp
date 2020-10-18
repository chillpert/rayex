#include "api/Surface.hpp"

#include "api/Components.hpp"
#include "api/utility/Util.hpp"

namespace RAYEXEC_NAMESPACE
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

    this->surface = surface;
    g_surface     = this->surface;
    RX_ASSERT( this->surface, "Failed to create surface." );
  }

  void Surface::assessSettings( )
  {
    // Get all surface capabilities.
    this->capabilities = g_physicalDevice.getSurfaceCapabilitiesKHR( this->surface );

    // Check a present mode.
    std::vector<vk::PresentModeKHR> presentModes = g_physicalDevice.getSurfacePresentModesKHR( this->surface );

    if ( !Util::find<vk::PresentModeKHR>( this->presentMode, presentModes ) )
    {
      Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eMailbox, presentModes ) ? this->presentMode                                                                                    = vk::PresentModeKHR::eMailbox :
                                                                                     Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eFifoRelaxed, presentModes ) ? this->presentMode = vk::PresentModeKHR::eFifoRelaxed :
                                                                                                                                                                        this->presentMode = vk::PresentModeKHR::eFifo;

      // Fall back, as FIFO is always supported on every device.
      RX_WARN( "Preferred present mode not available. Falling back to ", vk::to_string( this->presentMode ), " present mode." );
    }

    // Check format and color space.
    auto surfaceFormats = g_physicalDevice.getSurfaceFormatsKHR( this->surface );

    bool colorSpaceAndFormatSupported = false;
    for ( const auto& iter : surfaceFormats )
    {
      if ( iter.format == this->format && iter.colorSpace == this->colorSpace )
      {
        colorSpaceAndFormatSupported = true;
        break;
      }
    }

    // If the prefered format and color space are not available, fall back.
    if ( !colorSpaceAndFormatSupported )
    {
      this->format     = surfaceFormats[0].format;
      this->colorSpace = surfaceFormats[0].colorSpace;
      RX_WARN( "Preferred format and colorspace not supported. Falling back to the first option of each." );
    }

    g_surfaceFormat = this->format;
  }

  void Surface::destroy( )
  {
    if ( this->surface )
    {
      g_instance.destroySurfaceKHR( this->surface );
      this->surface = nullptr;
    }
  }
} // namespace RAYEXEC_NAMESPACE
