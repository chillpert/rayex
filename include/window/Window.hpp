#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "SDL2/SDL.h"

namespace RX
{
  class Window
  {
  public:
    Window(const char* title = "My application", int width = 600, int height = 400);

    bool initialize();
    void update();
    void render();
    void clean();

    const char* m_title;
    int m_width;
    int m_height;
  };
}

#endif // WINDOW_HPP