#include "Renderer.hpp"
#include "api/VulkanApi.hpp"

namespace RX
{
  Renderer::Renderer(API api)
  {
    switch (api)
    {
      case VULKAN:
        m_api = new VulkanApi();
        break;
    }
  }

  bool Renderer::initialize()
  {
    bool success = false;

    success = m_window.initialize();
    success = m_api->initialize();

    return success;
  }

  void Renderer::update()
  {
    m_window.update();
    m_api->update();
  }

  void Renderer::render()
  {
    m_window.update();
    m_api->update();
  }

  void Renderer::clean()
  {
    m_api->clean();
    m_window.clean();
  }
}