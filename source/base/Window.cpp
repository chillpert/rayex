#include "base/Window.hpp"

namespace RAYEXEC_NAMESPACE
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
    time.update( );

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

  auto Window::getInstanceExtensions( ) -> gsl::span<const char*>
  {
    uint32_t sdlExtensionsCount;
    SDL_bool result = SDL_Vulkan_GetInstanceExtensions( this->window, &sdlExtensionsCount, nullptr );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to get extensions required by SDL." );
    }

    gsl::owner<const char**> sdlExtensionsNames = new const char*[sdlExtensionsCount];
    result                                      = SDL_Vulkan_GetInstanceExtensions( this->window, &sdlExtensionsCount, sdlExtensionsNames );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to get extensions required by SDL." );
    }

    return gsl::span<const char*>( sdlExtensionsNames, sdlExtensionsCount );
  }

  auto Window::createSurface( vk::Instance instance ) -> vk::SurfaceKHR
  {
    VkSurfaceKHR surface;
    SDL_bool result = SDL_Vulkan_CreateSurface( this->window, instance, &surface );

    if ( result != SDL_TRUE )
    {
      RX_ERROR( "Failed to create surface" );
    }

    return surface;
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
} // namespace RAYEXEC_NAMESPACE