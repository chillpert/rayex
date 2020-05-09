#include "Renderer.hpp"
#include "api/Api.hpp"

namespace RX
{
  Renderer::Renderer() :
    m_window(std::make_shared<Window>()),
    m_api(m_window),
    m_initialized(false),
    m_running(true) { }

  Renderer::Renderer(std::shared_ptr<Window> window) :
    m_window(window),
    m_api(m_window),
    m_initialized(false),
    m_running(true) { }

  Renderer::~Renderer()
  {
    clean();
  }

  void Renderer::initialize()
  {
    if (m_initialized)
      return;

    m_window->initialize();
    m_api.initialize();

    m_initialized = true;
  }

  void Renderer::update()
  {
    if (!m_running || !m_initialized)
      return;

    m_running = m_window->update();
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
    initialize();

    while (m_running)
    {
      update();
      render();
    }
  }

  void Renderer::clean()
  {
    m_api.clean();
    m_window->clean();
  }
}