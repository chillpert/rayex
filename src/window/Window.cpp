#include "window/Window.hpp"

namespace RX
{
  Window::Window(WindowProperties windowProperties) :
    m_window(nullptr),
    m_properties{ windowProperties },
    m_time{ } { }

  Window::~Window()
  {
    clean();
  }

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
    // Update local timer bound to this window.
    m_time.update();

    // Fetch the latest window dimensions.
    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);
    m_properties.resize(width, height);

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

  std::vector<const char*> Window::getInstanceExtensions()
  {
    uint32_t sdlExtensionsCount;
    SDL_ASSERT(SDL_Vulkan_GetInstanceExtensions(m_window, &sdlExtensionsCount, nullptr), "Failed to get instance extensions count.");
    
    const char** sdlExtensionsNames = new const char* [sdlExtensionsCount];
    SDL_ASSERT(SDL_Vulkan_GetInstanceExtensions(m_window, &sdlExtensionsCount, sdlExtensionsNames), "Failed to get instance extensions.");

    std::vector<const char*> extensions;

    for (size_t i = 0; i < sdlExtensionsCount; ++i)
      extensions.push_back(sdlExtensionsNames[i]);

    return extensions;
  }

  VkSurfaceKHR Window::createSurface(vk::Instance instance)
  {
    VkSurfaceKHR surface;

    SDL_ASSERT(SDL_Vulkan_CreateSurface(m_window, instance, &surface), "Failed to create surface");

    return surface;
  }

  void Window::processGuiEvent(SDL_Event& event)
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
  }

  void Window::setTitle(const char* title)
  {
    m_properties.setTitle(title);
  }

  void Window::getSize(int* width, int* height)
  {
    SDL_GetWindowSize(m_window, width, height);
  }

  vk::Extent2D Window::getExtent()
  {
    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);
  
    return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
  }

  bool Window::changed()
  {
    static int prevWidth = m_properties.getWidth();
    static int prevHeight = m_properties.getHeight();

    int width = m_properties.getWidth();
    int height = m_properties.getHeight();

    if (width != prevWidth || height != prevHeight)
    {
      prevWidth = width;
      prevHeight = height;

      RX_LOG("Window refresh event");
      return true;
    }

    return false;
  }

  bool Window::minimized()
  {
    if (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED)
      return true;

    return false;
  }
}