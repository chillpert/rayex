#include "pch/stdafx.hpp"
#include "window/Window.hpp"
#include "events/Error.hpp"

#include <iostream>

namespace RX
{
  Window::Window(WindowProperties windowProperties)
    : m_window(nullptr), m_properties(windowProperties), m_time(Time()) { }

  void Window::initialize()
  {
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      Error::runtime(SDL_GetError(), Error::WINDOW);
    }

    m_window = SDL_CreateWindow(
      m_properties.getTitle(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      m_properties.getWidth(),
      m_properties.getHeight(),
      m_properties.getFlags()
    );
    
    if (m_window == nullptr)
    {
      Error::runtime("Failed to create window", Error::WINDOW);
    }
  }

  bool Window::update()
  {
    m_time.update();

    return true;
  }

  bool Window::render()
  {
    SDL_Event event;
    SDL_bool done = SDL_FALSE;

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          
          done = SDL_TRUE;
          break;

        case SDL_WINDOWEVENT:

          switch (event.window.event)
          {
            case SDL_WINDOWEVENT_CLOSE:
            {
              return false;
            }
            case SDL_WINDOWEVENT_RESIZED:
            {
              resize(static_cast<int>(event.window.data1), static_cast<int>(event.window.data2));
              break;
            }
          }

          break;
      }
    }

    return true;
  }

  void Window::clean()
  {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_Quit();
  }

  void Window::resize(int width, int height)
  {
    m_properties.resize(width, height);
    SDL_SetWindowSize(m_window, m_properties.getWidth(), m_properties.getHeight());
  }
}