#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <api/Api.hpp>

namespace RX
{
  enum API { VULKAN };

  class Renderer
  {
  public:
    RX_API Renderer(API api = VULKAN);

    RX_API bool initialize();
    RX_API void update();
    RX_API void render();
    RX_API void clean();

    RX_API inline Window* getWindow() { return &m_window; }

  private:
    Api* m_api;
    Window m_window;
  };
}

#endif // RENDERER_HPP