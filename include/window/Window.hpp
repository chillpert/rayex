#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "window/WindowProperties.hpp"

#include <SDL2/SDL.h>

namespace RX
{
  class Window
  {
  public:
    RX_API Window(int width, int height, const char* title);

    RX_API bool initialize();
    RX_API void update();
    RX_API void render();
    RX_API void clean();

    RX_API void resize();
  
  private:
    SDL_Window* m_window;
    WindowProperties m_properties;
  };
}

#endif // WINDOW_HPP