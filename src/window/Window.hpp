#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "pch/stdafx.hpp"
#include "window/WindowProperties.hpp"
#include "window/Time.hpp"

namespace RX
{
  class Window
  {
  public:
    RX_API Window(WindowProperties windowProperties = WindowProperties());
    RX_API virtual ~Window();

    RX_API virtual void initialize();
    RX_API virtual bool update();
    RX_API virtual bool render();
    RX_API virtual void clean();

    RX_API void resize(int width, int height);
    
    std::vector<const char*> getInstanceExtensions();

    RX_API VkSurfaceKHR createSurface(vk::Instance instance);

    RX_API void processGuiEvent(SDL_Event& event);

    RX_API void setTitle(const char* title);

    RX_API inline SDL_Window* get() { return m_window; }
    RX_API inline WindowProperties& getProperties() { return m_properties; }

    RX_API void getSize(int* width, int* height);
    RX_API vk::Extent2D getExtent();

    // Returns true if the window size changed.
    RX_API bool changed();
    RX_API bool minimized();
  
  protected:
    SDL_Window* m_window;
    
    WindowProperties m_properties;

    Time m_time;
  };
}

#endif // WINDOW_HPP