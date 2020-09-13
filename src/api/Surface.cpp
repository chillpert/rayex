#include "Surface.hpp"
#include "Components.hpp"

namespace rx
{
  Surface::Surface( vk::Format format, vk::ColorSpaceKHR colorSpace, vk::PresentModeKHR presentMode, bool initialize ) :
    m_format( format ),
    m_colorSpace( colorSpace ),
    m_presentMode( presentMode ),
    m_capabilities( 0 )
  {
    if ( initialize )
      init( );
  }

  Surface::~Surface( )
  {
    destroy( );
  }

  void Surface::init( )
  {
    m_surface = g_window->createSurface( g_instance );
    g_surface = m_surface;

    if ( !m_surface )
      RX_ERROR( "Failed to create surface." );
  }

  void Surface::checkSettingSupport( )
  {
    // Get all surface capabilities.
    m_capabilities = g_physicalDevice.getSurfaceCapabilitiesKHR( m_surface );

    // Check a present mode.
    std::vector<vk::PresentModeKHR> presentModes = g_physicalDevice.getSurfacePresentModesKHR( m_surface );

    bool presentModeSupported = false;
    for ( const auto& mode : presentModes )
    {
      if ( mode == m_presentMode )
      {
        presentModeSupported = true;
        break;
      }
    }

    // Fall back, as FIFO is always supported on every device.
    if ( !presentModeSupported )
    {
      m_presentMode = vk::PresentModeKHR::eFifo;
      RX_LOG( "Preferred present mode not available. Falling back to FIFO." );
    }

    // Check format and color space.
    auto formatProperties = g_physicalDevice.getFormatProperties( m_format );
    auto surfaceFormats = g_physicalDevice.getSurfaceFormatsKHR( m_surface );

    bool colorSpaceAndFormatSupported = false;
    for ( const auto& iter : surfaceFormats )
    {
      if ( iter.format == m_format && iter.colorSpace == m_colorSpace )
      {
        colorSpaceAndFormatSupported = true;
        break;
      }
    }

    // If the prefered format and color space are not available, fall back.
    if ( !colorSpaceAndFormatSupported )
    {
      m_format = surfaceFormats[0].format;
      m_colorSpace = surfaceFormats[0].colorSpace;
      RX_LOG( "Preferred format and colorspace not supported. Falling back to the first option of each." )
    }
    
    g_surfaceFormat = m_format;
  }

  void Surface::destroy( )
  {
    if ( m_surface )
    {
      g_instance.destroySurfaceKHR( m_surface );
      m_surface = nullptr;
    }
  }
}