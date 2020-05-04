#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "window/WindowProperties.hpp"
#include "window/Timer.hpp"

namespace RX
{
  class Window
  {
  public:
    Window(WindowProperties windowProperties);

    void initialize();
    void update();
    void render();
    void clean();

    RX_API void resize();
  
  private:
    SDL_Window* m_window;
    

    WindowProperties m_properties;

    Timer m_timer;
  };
}

#endif // WINDOW_HPP