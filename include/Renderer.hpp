#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <api/Api.hpp>

namespace RX
{
  class Renderer
  {
  public:
    RX_API Renderer(Api::Type api, WindowProperties windowProperties = WindowProperties());
    RX_API ~Renderer();

    RX_API void initialize();
    RX_API void update();
    RX_API void render();

    RX_API bool isRunning() { return m_running;  }

    void clean();

    RX_API inline const std::shared_ptr<Window> getWindow() const { return m_window; }

  private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Api> m_api;

    bool m_running;
  };
}

#endif // RENDERER_HPP