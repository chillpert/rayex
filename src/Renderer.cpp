#include "Renderer.hpp"

namespace RX
{
  Renderer::Renderer() :
    m_window(std::make_shared<WindowBase>()),
    m_camera(std::make_shared<CameraBase>(m_window->getSize())),
    m_api(m_window, m_camera),
    m_initialized(false),
    m_running(true) 
  {
    init();
  }

  Renderer::Renderer(std::shared_ptr<CameraBase> camera) :
    m_window(std::make_shared<WindowBase>()),
    m_camera(camera),
    m_api(m_window, m_camera),
    m_initialized(false),
    m_running(true) 
  {
    init();
  }

  Renderer::Renderer(std::shared_ptr<WindowBase> window) :
    m_window(window),
    m_camera(std::make_shared<CameraBase>(m_window->getSize())),
    m_api(window, m_camera),
    m_initialized(false),
    m_running(true) 
  {
    init();
  }

  Renderer::Renderer(std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera) :
    m_window(window),
    m_camera(camera),
    m_api(m_window, m_camera),
    m_initialized(false),
    m_running(true) 
  {
    init();
  }

  Renderer::Renderer(std::shared_ptr<WindowBase> window, std::shared_ptr<GuiBase> gui) :
    m_window(window),
    m_camera(std::make_shared<CameraBase>(m_window->getSize())),
    m_api(m_window, gui, m_camera),
    m_initialized(false),
    m_running(true) 
  {
    init();
  }

  Renderer::Renderer(std::shared_ptr<WindowBase> window, std::shared_ptr<GuiBase> gui, std::shared_ptr<CameraBase> camera) :
    m_window(window),
    m_camera(camera),
    m_api(m_window, gui, m_camera),
    m_initialized(false),
    m_running(true) 
  {
    init();
  }

  void Renderer::init()
  {
    if (m_initialized)
      return;

    m_window->init();
    m_api.init();

    m_initialized = true;
  }

  void Renderer::update()
  {
    if (!m_running || !m_initialized)
      return;

    m_running = m_window->update();
    m_camera->update();
    m_api.update();
  }

  void Renderer::render()
  {
    if (!m_running || !m_initialized)
      return;

    m_running = m_window->render();
    m_api.render();
  }

  void Renderer::start()
  {
    init();

    while (m_running)
    {
      update();
      render();
    }
  }

  void Renderer::pushNode(const std::shared_ptr<GeometryNodeBase> node)
  {
    m_api.pushNode(node);
  }

  void Renderer::setNodes(const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes)
  {
    m_api.setNodes(nodes);
  }
}