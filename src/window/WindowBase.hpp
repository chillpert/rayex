#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "pch/stdafx.hpp"
#include "window/WindowProperties.hpp"
#include "window/Time.hpp"

namespace rx
{
  class WindowBase
  {
  public:
    RX_API WindowBase( WindowProperties windowProperties = WindowProperties( ) );
    RX_API virtual ~WindowBase( );

    RX_API virtual void init( );
    RX_API virtual bool update( );
    RX_API virtual bool render( );
    RX_API virtual void clean( );

    RX_API void resize( int width, int height );

    std::vector<const char*> getInstanceExtensions( );

    RX_API vk::SurfaceKHR createSurface( vk::Instance instance );

    RX_API void processGuiEvent( SDL_Event& event );

    RX_API void setTitle( const char* title );

    RX_API inline SDL_Window* get( ) { return m_window; }
    RX_API inline WindowProperties& getProperties( ) { return m_properties; }

    RX_API void getSize( int* width, int* height );
    RX_API inline glm::ivec2 getSize( ) const { return glm::ivec2 { m_properties.getWidth( ), m_properties.getHeight( ) }; }
    RX_API vk::Extent2D getExtent( );

    // Returns true if the window size changed.
    RX_API bool changed( );
    RX_API bool minimized( );

  protected:
    SDL_Window* m_window;

    WindowProperties m_properties;

    Time m_time;
  };
}

#endif // WINDOW_HPP