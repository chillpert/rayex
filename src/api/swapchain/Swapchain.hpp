#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "api/Surface.hpp"
#include "api/Queues.hpp"

namespace RX
{
  struct SwapchainInfo
  {
    std::shared_ptr<Window> window;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;
    VkFormat surfaceFormat;
    VkColorSpaceKHR surfaceColorSpace;
    VkPresentModeKHR surfacePresentMode;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<uint32_t> queueFamilyIndices;
    VkExtent2D extent; // Ignore, will be initialized automatically
  };

  class Swapchain
  {
  public:
    ~Swapchain();

    inline VkSwapchainKHR get() { return m_swapchain; }    
    inline SwapchainInfo& getInfo() { return m_info; }

    void initialize(SwapchainInfo& info);
    void destroy();

    void acquireNextImage(VkDevice device, VkSemaphore semaphore, VkFence fence, uint32_t* imageIndex);
    
  private:
    VkSwapchainKHR m_swapchain;
    SwapchainInfo m_info;   
  };
}

#endif // SWAPCHAIN_HPP