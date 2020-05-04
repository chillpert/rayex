#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "window/WindowProperties.hpp"
#include "window/Time.hpp"

namespace RX
{
  class Window
  {
  public:
    Window(WindowProperties windowProperties);

    void initialize();
    bool update();
    bool render();
    void clean();

    RX_API void resize(int width, int height);
  
  private:
    SDL_Window* m_window;
    

    WindowProperties m_properties;

    Time m_time;
  };
}

#endif // WINDOW_HPP