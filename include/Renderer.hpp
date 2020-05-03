#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <api/Api.hpp>

namespace RX
{
  enum API { VULKAN };

  class Renderer
  {
  public:
    Renderer(API api = VULKAN);

    bool initialize();
    void update();
    void render();
    void clean();

    inline Window* getWindow() { return &m_window; }

  private:
    Api* m_api;
    Window m_window;
  };
}

#endif // RENDERER_HPP