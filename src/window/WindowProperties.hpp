#ifndef WINDOWPROPERTIES_HPP
#define WINDOWPROPERTIES_HPP

#include "Core.hpp"

#include <SDL2/SDL.h>
#include <cstdint>

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
  };

  class WindowProperties
  {
  public:
    RX_API WindowProperties(int width = 600, int height = 400, const char* title = "My Application", uint32_t flags = 0);

    inline int getWidth() { return m_width; }
    inline int getHeight() { return m_height; }

    inline const char* getTitle() { return m_title; }
    inline uint32_t getFlags() { return m_flags; }
   
    RX_API void resize(int width, int height);
    RX_API void setTitle(const char* title);

  private:
    int m_width;
    int m_height;

    const char* m_title;

    uint32_t m_flags;
  };
}

#endif // WINDOWPROPERTIES_HPP