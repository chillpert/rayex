#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <api/Api.hpp>

namespace RX
{
  enum class API { VULKAN };

  class Renderer
  {
  public:
    RX_API Renderer(API api, int width = 600, int height = 400, const char* title = "My application");

    RX_API bool initialize();
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