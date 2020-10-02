#include "api/Surface.hpp"
#include "api/misc/Components.hpp"
#include "api/utility/Util.hpp"

namespace RAYEXEC_NAMESPACE
{
  Surface::~Surface( )
  {
    destroy( );
  }

  void Surface::init( )
  {
    this->surface = g_window->createSurface( g_instance );
    g_surface = this->surface;
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
      Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eMailbox, presentModes ) ? this->presentMode = vk::PresentModeKHR::eMailbox :
      Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eImmediate, presentModes ) ? this->presentMode = vk::PresentModeKHR::eImmediate :
      Util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eFifoRelaxed, presentModes ) ? this->presentMode = vk::PresentModeKHR::eFifoRelaxed :
      this->presentMode = vk::PresentModeKHR::eFifo;

      std::string fallbackPresentMode;
      switch ( this->presentMode )
      {
        case vk::PresentModeKHR::eMailbox:
          fallbackPresentMode = "mailbox";
          break;

        case vk::PresentModeKHR::eImmediate:
          fallbackPresentMode = "immediate";
          break;

        case vk::PresentModeKHR::eFifoRelaxed:
          fallbackPresentMode = "fifo relaxed";
          break;

        case vk::PresentModeKHR::eFifo:
          fallbackPresentMode = "fifo";
          break;
      }

      // Fall back, as FIFO is always supported on every device.
      RX_WARN( "Preferred present mode not available. Falling back to ", fallbackPresentMode, " present mode." );
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
      this->format = surfaceFormats[0].format;
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
}