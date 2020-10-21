#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "base/Camera.hpp"
#include "base/Time.hpp"
#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  const int defaultWidth  = 900;
  const int defaultHeight = 600;

  /// Implements a SDL-based window.
  ///
  /// In order to handle events the user has to create a class that inherits from Window.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // This example requires the user to implement a custom window class that inherits from Window.
  /// auto myWindow = std::make_shared<CustomWindow>( width, height, "Example", WINDOW_RESIZABLE | WINDOW_INPUT_FOCUS );
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @see update()
  /// @ingroup BASE
  /// @todo To avoid the user having to link to SDL themselves requires to write a complete SDL2 wrapper.
  class RX_API Window
  {
  public:
    /// @param width The width of the window in pixels.
    /// @param height The height of the window in pixels.
    /// @param title The title of the window shown in the title bar.
    /// @param flags The window flags (no additional flags if not specified).
    Window( int width = defaultWidth, int height = defaultHeight, const char* title = "My Application", uint32_t flags = 0 );

    /// Destructor of Window.
    ///
    /// Will also clean up the object's resources.
    /// @see clean()
    virtual ~Window( );

    Window( const Window& )  = delete;
    Window( const Window&& ) = delete;
    auto operator=( const Window& ) -> Window& = delete;
    auto operator=( const Window && ) -> Window& = delete;

    /// Initializes the SDL-window.
    virtual auto init( ) -> bool;

    /// Updates window-related components.
    ///
    /// In case the user wants to handle input events, this function should be overwritten in the inherited class.
    /// @warning The user has to call Window::update() to ensure proper functionality.
    virtual auto update( ) -> bool;

    /// Destroys the window.
    virtual void clean( );

    /// Resizes the window.
    /// @param width The desired width in pixels.
    /// @param height The desired height in pixels.
    void resize( int width, int height );

    /// @return Returns the actual SDL_Window object.
    auto get( ) -> SDL_Window* { return _window; }

    /// @return Returns the window size as a Vulkan 2D extent.
    auto getExtent( ) const -> vk::Extent2D;

    /// @return Returns the window's width in pixels.
    auto getWidth( ) const -> int { return _width; }

    /// @return Returns the window's height in pixels.
    auto getHeight( ) const -> int { return _height; }

    /// @return Returns true if the window size has changed.
    auto changed( ) -> bool;

    /// @return Returns true if the window is minimized.
    auto minimized( ) -> bool;

  protected:
    SDL_Window* _window = nullptr; ///< The actual SDL_Window object.
    uint32_t _flags     = 0;       ///< The window's flags.

    int _width         = 1200;      ///< The window's width in pixels.
    int _height        = 600;       ///< The window's height in pixels.
    const char* _title = "Example"; ///< The window's title.
  };
} // namespace RAYEX_NAMESPACE

#endif // WINDOW_HPP
