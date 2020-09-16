#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "pch/stdafx.hpp"
#include "window/Time.hpp"
#include "base/CameraBase.hpp"

namespace RENDERER_NAMESPACE
{
  /// Flags for the window creation.
  /// 
  /// Acts like a wrapper for SDL_WindowFlags to only expose the most important ones.
  typedef enum WindowFlags
  {
    WINDOW_FULLSCREEN = 0x00000001, ///< The window is in window-fullscreen.
    WINDOW_BORDERLESS = 0x00000010, ///< Removes the window decoration.
    WINDOW_RESIZABLE = 0x00000020, ///< The window can be resized.
    WINDOW_MINIMIZED = 0x00000040, ///< The window is minimized.
    WINDOW_MAXIMIZED = 0x00000080, ///< The window is maximized.
    WINDOW_INPUT_FOCUS = 0x00000200, ///< The window has input focus.
    WINDOW_MOUSE_FOCUS = 0x00000400, ///< The window has mouse focus.
    WINDOW_FULLSCREEN_DESKTOP = ( WINDOW_FULLSCREEN | 0x00001000 ), ///< The window is in fullscreen.
    WINDOW_MOUSE_CAPTURE = 0x00004000 ///< Mouse can not leave the window.
  } WindowFlags;

  /// Implements a SDL-based window.
  /// 
  /// In order to handle events the client has to create a class that inherits from WindowBase.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // This example requires the client to implement a custom window class that inherits from WindowBase.
  /// auto myWindow = std::make_shared<CustomWindow>( width, height, "Example", WINDOW_RESIZABLE | WINDOW_INPUT_FOCUS );
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @see update()
  /// @ingroup Base
  /// @todo To avoid the client having to link to SDL themselves requires to write a complete SDL2 wrapper.
  class WindowBase
  {
  public:
    /// @param width The width of the window in pixels.
    /// @param height The height of the window in pixels.
    /// @param title The title of the window shown in the title bar.
    /// @param flags The window flags (no additional flags if not specified).
    RX_API WindowBase( int width = 600, int height = 400, const char* title = "My Application", uint32_t flags = 0 );

    /// Destructor of WindowBase.
    /// 
    /// Will also clean up the object's resources.
    /// @see clean()
    RX_API virtual ~WindowBase( );

    /// Initializes the SDL-window.
    RX_API virtual void init( );

    /// Updates window-related components.
    /// 
    /// In case the client wants to handle input events, this function should be overwritten in the inherited class.
    /// @warning The client has to call WindowBase::update() to ensure proper functionality.
    RX_API virtual bool update( );

    /// Destroys the window.
    RX_API virtual void clean( );

    /// Resizes the window.
    /// @param width The desired width in pixels.
    /// @param height The desired height in pixels.
    RX_API void resize( int width, int height );

    /// Used to retrieve all Vulkan instance extensions required by SDL.
    /// @return Returns a vector containing the names of the required extensions.
    std::vector<const char*> getInstanceExtensions( );

    /// Creates a Vulkan surface.
    /// @param instance The Vulkan instance.
    /// @return Returns the Vulkan surface.
    RX_API vk::SurfaceKHR createSurface( vk::Instance instance );

    /// @return Returns the actual SDL_Window object.
    RX_API inline SDL_Window* get( ) { return m_window; }

    /// @return Returns the window size as a Vulkan 2D extent.
    RX_API vk::Extent2D getExtent( ) const;

    /// @return Returns the window's width in pixels.
    inline int getWidth( ) const { return m_width; }

    /// @return Returns the window's height in pixels.
    inline int getHeight( ) const { return m_height; }

    /// @return Returns true if the window size has changed.
    RX_API bool changed( );

    /// @return Returns true if the window is minimized.
    RX_API bool minimized( );

  protected:
    SDL_Window* m_window; ///< The actual SDL_Window object.

    int m_width; ///< The window's width in pixels.
    int m_height; ///< The window's height in pixels.
    const char* m_title; ///< The window's title.
    uint32_t m_flags; ///< The window's flags.

    Time m_time; ///< The timer that manages the application's timing.
  };
}

#endif // WINDOW_HPP