#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "api/BaseComponent.hpp"
#include "api/Surface.hpp"
#include "api/QueueManager.hpp"

namespace RX
{
  class Swapchain : public BaseComponent
  {
  public:
    Swapchain();
    ~Swapchain();

    inline VkSwapchainKHR get() { return m_swapchain; }    
    inline VkExtent2D& getExtent() { return m_extent; } 

    void initialize(VkPhysicalDevice physicalDevice, VkDevice device, Surface& surface, std::shared_ptr<Window> window, QueueManager& queueManager);
    void destroy();
    
  private:
    VkSwapchainKHR m_swapchain; // TODO: rename
    VkExtent2D m_extent; // TODO: rename
    VkDevice m_device;
  };
}

#endif // SWAPCHAIN_HPP