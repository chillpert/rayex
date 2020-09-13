#include "WindowBase.hpp"

namespace rx
{
  WindowBase::WindowBase( int width, int height, const char* title, uint32_t flags ) :
    m_window( nullptr ),
    m_width( width ),
    m_height( height ),
    m_title( title ),
    m_flags( flags )
  {
    m_flags |= SDL_WINDOW_VULKAN;
  }

  WindowBase::~WindowBase( )
  {
    clean( );
  }

  void WindowBase::init( )
  {
    SDL_SetHint( SDL_HINT_FRAMEBUFFER_ACCELERATION, "1" );

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
      RX_ERROR( SDL_GetError( ) );

    m_window = SDL_CreateWindow( m_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, m_flags );

    if ( m_window == nullptr )
      RX_ERROR( "Failed to create window." ); 
  }

  bool WindowBase::update( )
  {
    // Updates local timer bound to this window.
    m_time.update( );
    
    // Fetch the latest window dimensions.
    int width, height;
    SDL_GetWindowSize( m_window, &width, &height );
    resize( width, height );

    return true;
  }

  void WindowBase::clean( )
  {
    SDL_DestroyWindow( m_window );
    m_window = nullptr;

    SDL_Quit( );
  }

  void WindowBase::resize( int width, int height )
  {
    m_width = width;
    m_height = height;

#ifndef RX_PLATFORM_UNIX_X64
    SDL_SetWindowSize( m_window, m_width, m_height );
#endif
  }

  std::vector<const char*> WindowBase::getInstanceExtensions( )
  {
    uint32_t sdlExtensionsCount;
    SDL_bool result = SDL_Vulkan_GetInstanceExtensions( m_window, &sdlExtensionsCount, nullptr );

    if ( result != SDL_TRUE )
      RX_ERROR( "Failed to get extensions required by SDL." );

    const char** sdlExtensionsNames = new const char* [sdlExtensionsCount];
    result = SDL_Vulkan_GetInstanceExtensions( m_window, &sdlExtensionsCount, sdlExtensionsNames );

    if ( result != SDL_TRUE )
      RX_ERROR( "Failed to get extensions required by SDL." );

    std::vector<const char*> extensions;
    extensions.reserve( sdlExtensionsCount );

    for ( size_t i = 0; i < sdlExtensionsCount; ++i )
      extensions.push_back( sdlExtensionsNames[i] );

    return extensions;
  }

  vk::SurfaceKHR WindowBase::createSurface( vk::Instance instance )
  {
    VkSurfaceKHR surface;
    SDL_bool result = SDL_Vulkan_CreateSurface( m_window, instance, &surface );

    if ( result != SDL_TRUE )
      RX_ERROR( "Failed to create surface" );

    return surface;
  }

  vk::Extent2D WindowBase::getExtent( ) const
  {
    int width, height;
    SDL_GetWindowSize( m_window, &width, &height );

    return { static_cast<uint32_t>( width ), static_cast<uint32_t>( height ) };
  }

  bool WindowBase::changed( )
  {
    static int prevWidth = m_width;
    static int prevHeight = m_height;

    if ( m_width != prevWidth || m_height != prevHeight )
    {
      prevWidth = m_width;
      prevHeight = m_height;
      return true;
    }

    return false;
  }

  bool WindowBase::minimized( )
  {
    if ( SDL_GetWindowFlags( m_window ) & SDL_WINDOW_MINIMIZED )
      return true;

    return false;
  }
}