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

    inline VkSwapchainKHR get() { return swapchain; }    
    inline VkExtent2D& getExtent() { return extent; } 

    void initialize(VkPhysicalDevice physicalDevice, VkDevice device, Surface surface, std::shared_ptr<Window> window, QueueManager& queueManager);
    void destroy(VkDevice device);
    
  private:
    VkSwapchainKHR swapchain;
    VkExtent2D extent;
  };
}

#endif // SWAPCHAIN_HPP