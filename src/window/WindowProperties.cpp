#include "window/WindowProperties.hpp"
#include "api/Api.hpp"

namespace RX
{
  WindowProperties::WindowProperties(int width, int height, const char* title, uint32_t flags)
    : m_width(width), m_height(height), m_title(title), m_flags(flags)
  {
    switch (Api::getType())
    {
      case Api::VULKAN:
        m_flags |= SDL_WINDOW_VULKAN;
        break;
    }
  }

  void WindowProperties::resize(int width, int height)
  {
    m_width = width;
    m_height = height;
  }

  void WindowProperties::setTitle(const char* title)
  {
    m_title = title;
  }
}