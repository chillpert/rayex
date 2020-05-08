#include "api/Surface.hpp"

namespace RX
{
  void Surface::createSurface(Window* window, VkInstance* instance)
  {
    m_instance = instance;

    Assert::sdl(SDL_Vulkan_CreateSurface(window->getWindow(), *m_instance, &m_surface), "Failed to create surface");
  }

  void Surface::destroySurface()
  {
    vkDestroySurfaceKHR(*m_instance, m_surface, nullptr);
  }
}