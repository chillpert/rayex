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
    virtual ~Window() = default;

    RX_API virtual void initialize();
    RX_API virtual bool update();
    RX_API virtual bool render();
    RX_API virtual void clean();

    RX_API void resize(int width, int height);
    RX_API void resizeFramebuffer(int width, int height);

    RX_API void getInstanceExtensions(uint32_t& count, const char** extensions);
    RX_API VkSurfaceKHR createSurface(VkInstance instance);

    RX_API void setTitle(const char* title);

    RX_API inline SDL_Window* getWindow() { return m_window; }
    RX_API inline WindowProperties& getProperties() { return m_properties; }
    RX_API void getSize(int* width, int* height);
    RX_API VkExtent2D getExtent();
  
  protected:
    SDL_Window* m_window;
    
    WindowProperties m_properties;

    Time m_time;
  };
}

#endif // WINDOW_HPP