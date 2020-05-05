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
    RX_API void setTitle(const char* title);
    RX_API inline WindowProperties& getProperties() { return m_properties; }
  
  private:
    SDL_Window* m_window;
    
    WindowProperties m_properties;

    Time m_time;
  };
}

#endif // WINDOW_HPP