#include "base/Window.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  Window::Window( int width, int height, const char* title, uint32_t flags ) :

    width( width ),
    height( height ),
    title( title ),
    flags( flags )
  {
    this->flags |= SDL_WINDOW_VULKAN;
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

    this->window = SDL_CreateWindow( this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, this->flags );

    if ( this->window == nullptr )
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
    SDL_GetWindowSize( this->window, &width, &height );
    resize( width, height );

    return true;
  }

  void Window::clean( )
  {
    SDL_DestroyWindow( this->window );
    this->window = nullptr;

    SDL_Quit( );
  }

  void Window::resize( int width, int height )
  {
    this->width  = width;
    this->height = height;

#if defined( _WIN32 ) || defined( _WIN64 )
    SDL_SetWindowSize( this->window, this->width, this->height );
#endif
  }

  auto Window::getExtent( ) const -> vk::Extent2D
  {
    int width;
    int height;
    SDL_GetWindowSize( this->window, &width, &height );

    return { static_cast<uint32_t>( width ), static_cast<uint32_t>( height ) };
  }

  auto Window::changed( ) -> bool
  {
    static int prevWidth  = this->width;
    static int prevHeight = this->height;

    if ( this->width != prevWidth || this->height != prevHeight )
    {
      // Frame counter for jitter cam needs to be reset.
      g_frameCount = 0;

      prevWidth  = this->width;
      prevHeight = this->height;
      return true;
    }

    return false;
  }

  auto Window::minimized( ) -> bool
  {
    return ( ( SDL_GetWindowFlags( this->window ) & SDL_WINDOW_MINIMIZED ) != 0U );
  }
} // namespace RAYEX_NAMESPACE
