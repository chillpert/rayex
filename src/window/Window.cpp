#include "window/Window.hpp"

#include <iostream>

namespace RX
{
  Window::Window(const char* title, int width, int height)
    : m_title(title), m_width(width), m_height(height) { }

  bool Window::initialize()
  {
    /*
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      std::cerr << "Renderer: Errir initializing SDL2: " << SDL_GetError() << std::endl;
      return false;
    }

    SDL_Window* win = SDL_CreateWindow(
      m_title,
      SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 
      m_width, 
      m_height,
      0
    );
    */
  }

  void Window::update()
  {

  }

  void Window::render()
  {

  }

  void Window::clean()
  {

  }
}