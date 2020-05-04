#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <api/Api.hpp>

namespace RX
{
  class Renderer
  {
  public:
    RX_API Renderer(Api::Type api, WindowProperties windowProperties = WindowProperties());

    RX_API void initialize();
    RX_API void update();
    RX_API void render();
    RX_API void clean();

    RX_API inline std::shared_ptr<Window> getWindow() { return m_window; }

  private:
    std::shared_ptr<Api> m_api;
    std::shared_ptr<Window> m_window;
  };
}

#endif // RENDERER_HPP