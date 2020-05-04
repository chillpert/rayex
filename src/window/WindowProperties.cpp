#include "window/WindowProperties.hpp"

namespace RX
{
  WindowProperties::WindowProperties(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(title) { }

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