#include "api/Surface.hpp"

namespace RX
{
  void Surface::createSurface(std::shared_ptr<Window> window, vk::UniqueInstance* instance)
  {
    m_instance = instance;

    Assert::sdl(SDL_Vulkan_CreateSurface(window->getWindow(), m_instance->get(), &m_surface), "Failed to create surface");
  }

  void Surface::destroySurface()
  {
    vkDestroySurfaceKHR(m_instance->get(), m_surface, nullptr);
  }
}