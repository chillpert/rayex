#ifndef WINDOWPROPERTIES_HPP
#define WINDOWPROPERTIES_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  typedef enum WindowFlags
  {
    WINDOW_FULLSCREEN = 0x00000001,
    WINDOW_VISIBLE = 0x00000004,
    WINDOW_HIDDEN = 0x00000008,
    WINDOW_BORDERLESS = 0x00000010,
    WINDOW_RESIZABLE = 0x00000020,
    WINDOW_MINIMIZED = 0x00000040,
    WINDOW_MAXIMIZED = 0x00000080,
    WINDOW_INPUT_GRABBED = 0x00000100,
    WINDOW_INPUT_FOCUS = 0x00000200,
    WINDOW_MOUSE_FOCUS = 0x00000400,
    WINDOW_FULLSCREEN_DESKTOP = (WINDOW_FULLSCREEN | 0x00001000),
    WINDOW_MOUSE_CAPTURE = 0x00004000
  } WindowFlags;

  class WindowProperties
  {
  public:
    RX_API WindowProperties(int width = 600, int height = 400, const char* title = "My Application", uint32_t flags = 0);

    RX_API inline int getWidth() const { return m_width; } 
    RX_API inline int getHeight() const { return m_height; }

    RX_API inline const char* getTitle() const { return m_title; }
    inline const char* getEngineTitle() const { return m_engineTitle; }

    RX_API inline uint32_t getFlags() const { return m_flags; }
   
    void resize(int width, int height);
    void resizeFramebuffer(int width, int height);
    void setTitle(const char* title);
    void setEngineTitle(const char* title);

  private:
    int m_width;
    int m_height;

    const char* m_title;
    const char* m_engineTitle;

    uint32_t m_flags;
  };
}

#endif // WINDOWPROPERTIES_HPP