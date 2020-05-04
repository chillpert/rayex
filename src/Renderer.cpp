#include "Renderer.hpp"
#include "api/VulkanApi.hpp"

namespace RX
{
  Renderer::Renderer(API api, int width, int height, const char* title)
  {
    switch (api)
    {
      case API::VULKAN:
        m_api = std::make_shared<VulkanApi>();
        break;
    }

    m_window = std::make_shared<Window>(width, height, title);
  }

  bool Renderer::initialize()
  {
    bool success = false;

    success = m_window->initialize();
    success = m_api->initialize(m_window);

    return success;
  }

  void Renderer::update()
  {
    m_window->update();
    m_api->update();
  }

  void Renderer::render()
  {
    m_window->update();
    m_api->update();
  }

  void Renderer::clean()
  {
    m_api->clean();
    m_window->clean();
  }
}