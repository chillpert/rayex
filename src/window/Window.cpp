#include "window/Window.hpp"

#include <vulkan/vulkan.h>
#include <iostream>

namespace RX
{
  Window::Window(WindowProperties windowProperties)
    : m_window(nullptr), m_properties(windowProperties), m_timer(Timer()) { }

  void Window::initialize()
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      std::cerr << "Renderer: Error initializing SDL2: " << SDL_GetError() << std::endl;
      
    }

    m_window = SDL_CreateWindow(
      m_properties.getTitle(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      m_properties.getWidth(),
      m_properties.getHeight(),
      m_properties.getFlags()
    );
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

  void Window::resize()
  {
    
  }
}