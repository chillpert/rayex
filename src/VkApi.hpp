#ifndef VK_API_HPP
#define VK_API_HPP

#include "window/Window.hpp"

namespace RX
{
  class VkApi
  {
  public:
    VkApi(std::shared_ptr<Window> window);

    void initialize();
    bool update();
    bool render();
    void clean();
  
  private:
    std::shared_ptr<Window> m_window;

    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapChain;
    VkSemaphore m_semaphore;
    VkQueue m_queue;
  };
}

#endif // VK_API_HPP