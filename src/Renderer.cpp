#include "Renderer.hpp"
#include "api/Api.hpp"

namespace RX
{
  Renderer::Renderer(WindowProperties windowProperties) : 
    m_window(Window(windowProperties)), 
    m_api{ }, 
    m_running(true)
  {
    initialize();
  }

  Renderer::~Renderer()
  {
    clean();
  }

  void Renderer::initialize()
  {
    m_window.initialize();
    m_api.initialize(&m_window);
  }

  void Renderer::update()
  {
    m_running = m_window.update();
    m_api.update();
  }

  void Renderer::render()
  {
    m_running = m_window.render();
    m_api.render();
  }

  void Renderer::clean()
  {
    m_api.clean();
    m_window.clean();
  }
}