#include "Renderer.hpp"
#include "api/vulkan/VulkanApi.hpp"

namespace RX
{
  Renderer::Renderer(WindowProperties windowProperties)
    : m_running(true)
  {
    m_window = std::make_shared<Window>(windowProperties);

#ifdef RX_VULKAN
    m_api = std::make_shared<VulkanApi>();
#endif

    initialize();
  }

  Renderer::~Renderer()
  {
    clean();
  }

  void Renderer::initialize()
  {
    m_window->initialize();
    m_api->initialize(m_window);
  }

  void Renderer::update()
  {
    m_running = m_window->update();
    m_api->update();
  }

  void Renderer::render()
  {
    m_running = m_window->render();
    m_api->render();
  }

  void Renderer::clean()
  {
    m_api->clean();
    m_window->clean();
  }
}