#include "window/Window.hpp"
#include "Camera.hpp"

namespace RX
{
  Window::Window(WindowProperties windowProperties) :
    m_window(nullptr),
    m_properties{ windowProperties },
    m_time{ } { }

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

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          return false;

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

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym)
          {
            case SDLK_w:
              std::cout << "w pressed" << std::endl;
              break;

            case SDLK_a:

              break;

            case SDLK_s:

              break;

            case SDLK_d:

              break;

            case SDLK_c:

              break;

            case SDLK_SPACE:

              break;
          }
          break;

        case SDL_KEYUP:
          switch (event.key.keysym.sym)
          {
          case SDLK_w:

            break;

          case SDLK_a:

            break;

          case SDLK_s:

            break;

          case SDLK_d:

            break;

          case SDLK_c:

            break;

          case SDLK_SPACE:

            break;
          }
          break;

        case SDL_MOUSEMOTION:

          break;

        case SDL_MOUSEBUTTONDOWN:
          
          break;

        case SDL_MOUSEBUTTONUP:

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

  void Window::setTitle(const char* title)
  {
    m_properties.setTitle(title);
  }
}