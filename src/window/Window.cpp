#include "window/Window.hpp"

namespace RX
{
  Window::Window(WindowProperties windowProperties) :
    m_window(nullptr),
    m_properties{ windowProperties },
    m_time{ } { }

  void Window::initialize()
  {
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      RX_ERROR(SDL_GetError());

    m_window = SDL_CreateWindow(
      m_properties.getTitle(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      m_properties.getWidth(),
      m_properties.getHeight(),
      m_properties.getFlags()
    );
    
    if (m_window == nullptr)
      RX_ERROR("Failed to create window.");
  }

  bool Window::update()
  {
    m_time.update();

    return true;
  }

  bool Window::render()
  {
    return true;
  }

  void Window::clean()
  {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_Quit();
  }

  void Window::resize(int width, int height)
  {
    m_properties.resize(width, height);
    SDL_SetWindowSize(m_window, m_properties.getWidth(), m_properties.getHeight());
  }

  void Window::resizeFramebuffer(int width, int height)
  {
    m_properties.resizeFramebuffer(width, height);
  }

  void Window::getInstanceExtensions(uint32_t& count, const char** extensions)
  {
    if (extensions == NULL)
    {
      if (!SDL_Vulkan_GetInstanceExtensions(m_window, &count, NULL))
        RX_ERROR("SDL failed to get instance extensions");
    }
    else
    {
      if (!SDL_Vulkan_GetInstanceExtensions(m_window, &count, extensions))
        RX_ERROR("SDL failed to get instance extensions");
    }
  }

  VkSurfaceKHR Window::createSurface(VkInstance instance)
  {
    VkSurfaceKHR surface;

    SDL_ASSERT(SDL_Vulkan_CreateSurface(m_window, instance, &surface), "Failed to create surface");

    return surface;
  }

  void Window::setTitle(const char* title)
  {
    m_properties.setTitle(title);
  }

  void Window::getSize(int* width, int* height)
  {
    SDL_GetWindowSize(m_window, width, height);
  }

  VkExtent2D Window::getExtent()
  {
    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);
  
    return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
  }
}