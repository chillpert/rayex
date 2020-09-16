#include "api/Surface.hpp"
#include "api/misc/Components.hpp"
#include "api/utility/Helpers.hpp"

namespace RENDERER_NAMESPACE
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

    if ( !util::find<vk::PresentModeKHR>( m_presentMode, presentModes ) )
    {
      util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eMailbox, presentModes ) ? m_presentMode = vk::PresentModeKHR::eMailbox :
      util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eImmediate, presentModes ) ? m_presentMode = vk::PresentModeKHR::eImmediate :
      util::find<vk::PresentModeKHR>( vk::PresentModeKHR::eFifoRelaxed, presentModes ) ? m_presentMode = vk::PresentModeKHR::eFifoRelaxed :
      m_presentMode = vk::PresentModeKHR::eFifo;

      std::string fallbackPresentMode;
      switch ( m_presentMode )
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
      RX_WARN( "Preferred format and colorspace not supported. Falling back to the first option of each." );
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