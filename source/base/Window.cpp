#include "base/Window.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  Window::Window( int width, int height, const char* title, uint32_t flags ) :

    _width( width ),
    _height( height ),
    _title( title ),
    _flags( flags )
  {
    _flags |= SDL_WINDOW_VULKAN;
  }

  Window::~Window( )
  {
    clean( );
  }

  auto Window::init( ) -> bool
  {
    //SDL_SetHint( SDL_HINT_FRAMEBUFFER_ACCELERATION, "1" );
    //SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
      RX_ERROR( "SDL Error: ", SDL_GetError( ), "\nClosing application." );
      return false;
    }

    _window = SDL_CreateWindow( _title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, _flags );

    if ( _window == nullptr )
    {
      RX_ERROR( "Failed to create window. Closing application." );
      return false;
    }

    //if ( SDL_GL_SetSwapInterval( 1 ) == -1 )
    //{
    //  RX_ERROR( "Swap interval not supported." );
    //  return false;
    //}

    return true;
  }

  auto Window::update( ) -> bool
  {
    // Updates local timer bound to this window.
    Time::update( );

    // Fetch the latest window dimensions.
    int width;
    int height;
    SDL_GetWindowSize( _window, &width, &height );
    resize( width, height );

    return true;
  }

  void Window::clean( )
  {
    SDL_DestroyWindow( _window );
    _window = nullptr;

    SDL_Quit( );
  }

  void Window::resize( int width, int height )
  {
    _width  = width;
    _height = height;

#if defined( _WIN32 ) || defined( _WIN64 )
    SDL_SetWindowSize( _window, _width, _height );
#endif
  }

  auto Window::getExtent( ) const -> vk::Extent2D
  {
    int width;
    int height;
    SDL_GetWindowSize( _window, &width, &height );

    return { static_cast<uint32_t>( width ), static_cast<uint32_t>( height ) };
  }

  auto Window::changed( ) -> bool
  {
    static int prevWidth  = _width;
    static int prevHeight = _height;

    if ( _width != prevWidth || _height != prevHeight )
    {
      // Frame counter for jitter cam needs to be reset.
      components::frameCount = -1;

      prevWidth  = _width;
      prevHeight = _height;
      return true;
    }

    return false;
  }

  auto Window::minimized( ) -> bool
  {
    return ( ( SDL_GetWindowFlags( _window ) & SDL_WINDOW_MINIMIZED ) != 0U );
  }

  auto Window::getExtensions( ) const -> gsl::span<const char*>
  {
    // Retrieve all extensions needed by SDL2.
    uint32_t sdlExtensionsCount;
    SDL_bool result = SDL_Vulkan_GetInstanceExtensions( _window, &sdlExtensionsCount, nullptr );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to get extensions required by SDL." );
    }

    gsl::owner<const char**> sdlExtensionsNames = new const char*[sdlExtensionsCount];
    result                                      = SDL_Vulkan_GetInstanceExtensions( _window, &sdlExtensionsCount, sdlExtensionsNames );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to get extensions required by SDL." );
    }

    return gsl::span<const char*>( sdlExtensionsNames, sdlExtensionsCount );
  }
} // namespace RAYEX_NAMESPACE
