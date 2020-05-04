#include "Renderer.hpp"
#include "api/VulkanApi.hpp"

namespace RX
{
  Renderer::Renderer(Api::Type api, WindowProperties windowProperties)
  {
    switch (api)
    {
    case Api::VULKAN:
        api = Api::VULKAN;
        m_api = std::make_shared<VulkanApi>();
        break;
    }

    m_window = std::make_shared<Window>(windowProperties);
  }

  void Renderer::initialize()
  {
    m_window->initialize();
    m_api->initialize(m_window);
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