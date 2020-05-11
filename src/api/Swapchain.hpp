#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "Surface.hpp"

namespace RX
{
  class Swapchain
  {
  public:
    inline VkSwapchainKHR get() { return swapchain; }
    
    void create(VkPhysicalDevice physicalDevice, VkDevice device, Surface surface, std::shared_ptr<Window> window, uint32_t* familyIndex);
    void destroy(VkDevice device);

  private:
    VkSwapchainKHR swapchain;
  };
}

#endif // SWAPCHAIN_HPP