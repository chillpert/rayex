#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "api/Api.hpp"

namespace RX
{
  class Renderer
  {
  public:
    RX_API Renderer();
    RX_API Renderer(std::shared_ptr<Window> window, std::shared_ptr<Gui> gui = nullptr);
    RX_API ~Renderer() = default;

    RX_API void initialize();
    RX_API void update();
    RX_API void render();

    RX_API void start();

    RX_API bool isRunning() { return m_running;  }

    RX_API inline const std::shared_ptr<Window> getWindow() const { return m_window; }
    RX_API inline const std::shared_ptr<Gui> getGui() const { return m_gui; }

    RX_API void clearModels();
    RX_API void pushModel(const std::shared_ptr<Model> model);
    RX_API void setModels(const std::vector<std::shared_ptr<Model>>& models);

  private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Gui> m_gui;
    Api m_api;

    bool m_initialized;
    bool m_running;
  };
}

#endif // RENDERER_HPP