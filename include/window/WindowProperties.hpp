#ifndef WINDOWPROPERTIES_HPP
#define WINDOWPROPERTIES_HPP

#include "Export.hpp"
#include "window/Timer.hpp"

namespace RX
{
  class WindowProperties
  {
  public:
    WindowProperties(int width, int height, const char* title);

    inline int getWidth() { return m_width; }
    inline int getHeight() { return m_height; }

    inline const char* getTitle() { return m_title; }

    void resize(int width, int height);
    void setTitle(const char* title);

  private:
    int m_width;
    int m_height;

    const char* m_title;

    Timer time;
  };
}

#endif // WINDOWPROPERTIES_HPP