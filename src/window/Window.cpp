#include "window/Window.hpp"

#include <vulkan/vulkan.h>
#include <iostream>

namespace RX
{
  Window::Window(int width, int height, const char* title)
    : m_window(nullptr), m_properties(WindowProperties(width, height, title)) { }

  bool Window::initialize()
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      std::cerr << "Renderer: Error initializing SDL2: " << SDL_GetError() << std::endl;
      return false;
    }

    m_window = SDL_CreateWindow(
      m_properties.getTitle(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      m_properties.getWidth(),
      m_properties.getHeight(),
      0
    );

    if (m_window == nullptr)
      return false;

    return true;
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