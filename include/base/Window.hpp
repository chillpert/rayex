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
  /// @ingroup Base
  /// @todo To avoid the user having to link to SDL themselves requires to write a complete SDL2 wrapper.
  class Window
  {
  public:
    /// @param width The width of the window in pixels.
    /// @param height The height of the window in pixels.
    /// @param title The title of the window shown in the title bar.
    /// @param flags The window flags (no additional flags if not specified).
    RX_API Window( int width = defaultWidth, int height = defaultHeight, const char* title = "My Application", uint32_t flags = 0 );

    /// Destructor of Window.
    ///
    /// Will also clean up the object's resources.
    /// @see clean()
    RX_API virtual ~Window( );

    Window( const Window& )  = delete;
    Window( const Window&& ) = delete;
    auto operator=( const Window& ) -> Window& = delete;
    auto operator=( const Window && ) -> Window& = delete;

    /// Initializes the SDL-window.
    RX_API virtual auto init( ) -> bool;

    /// Updates window-related components.
    ///
    /// In case the user wants to handle input events, this function should be overwritten in the inherited class.
    /// @warning The user has to call Window::update() to ensure proper functionality.
    RX_API virtual auto update( ) -> bool;

    /// Destroys the window.
    RX_API virtual void clean( );

    /// Resizes the window.
    /// @param width The desired width in pixels.
    /// @param height The desired height in pixels.
    RX_API void resize( int width, int height );

    /// @return Returns the actual SDL_Window object.
    RX_API auto get( ) -> SDL_Window* { return window; }

    /// @return Returns the window size as a Vulkan 2D extent.
    [[nodiscard]] RX_API auto getExtent( ) const -> vk::Extent2D;

    /// @return Returns the window's width in pixels.
    [[nodiscard]] auto getWidth( ) const -> int { return width; }

    /// @return Returns the window's height in pixels.
    [[nodiscard]] auto getHeight( ) const -> int { return height; }

    /// @return Returns true if the window size has changed.
    RX_API auto changed( ) -> bool;

    /// @return Returns true if the window is minimized.
    RX_API auto minimized( ) -> bool;

  protected:
    SDL_Window* window = nullptr; ///< The actual SDL_Window object.
    uint32_t flags;               ///< The window's flags.

    int width;         ///< The window's width in pixels.
    int height;        ///< The window's height in pixels.
    const char* title; ///< The window's title.
  };
} // namespace RAYEX_NAMESPACE

#endif // WINDOW_HPP
