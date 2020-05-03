#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "window/WindowProperties.hpp"

#include <SDL2/SDL.h>

namespace RX
{
  class Window
  {
  public:
    bool initialize();
    void update();
    void render();
    void clean();

    void resize();
  
  private:
    WindowProperties m_properties;
  };
}

#endif // WINDOW_HPP