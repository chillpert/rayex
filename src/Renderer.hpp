#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "api/Api.hpp"
#include "Base.hpp"

namespace RX
{
  class Renderer
  {
  public:
    RX_API Renderer();
    RX_API Renderer(std::shared_ptr<CameraBase> camera);
    RX_API Renderer(std::shared_ptr<Window> window);
    RX_API Renderer(std::shared_ptr<Window> window, std::shared_ptr<CameraBase> camera);
    RX_API Renderer(std::shared_ptr<Window> window, std::unique_ptr<Gui> gui);
    RX_API Renderer(std::shared_ptr<Window> window, std::unique_ptr<Gui> gui, std::shared_ptr<CameraBase> camera);
    RX_API ~Renderer() = default;

    RX_API void initialize();
    RX_API void update();
    RX_API void render();

    RX_API void start();

    RX_API bool isRunning() { return m_running;  }

    RX_API inline const std::shared_ptr<Window> getWindow() const { return m_window; }

    RX_API void pushNode(const std::shared_ptr<GeometryNodeBase> node);
    RX_API void setNodes(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes);

  private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<CameraBase> m_camera;
    Api m_api;

    bool m_initialized;
    bool m_running;
  };
}

#endif // RENDERER_HPP