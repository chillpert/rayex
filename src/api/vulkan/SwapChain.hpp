#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "pch/stdafx.hpp"
#include "window/WindowProperties.hpp"

namespace RX
{
  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  class SwapChain
  {
  public:
    SwapChain(VkSurfaceKHR* surface, WindowProperties* windowProperties);

    void createSwapChain(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice);
    void createImageViews();

    void destroySwapChain();
    void destroyImageView();

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static inline VkSurfaceKHR* getSurface() { return s_surface; }

  private: 
    // Returns the surface format and color space.
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    // Returns the presentation mode.
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    // Returns the swap extent containing the resolution of the swap chain images.
    // If VK_PRESENT_MODE_MAILBOX_KHR is not available on the device, then it will choose VK_PRESENT_MODE_FIFO_KHR
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;

    // swap chain properties
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    // pointer to VulkanApi class members
    static VkSurfaceKHR* s_surface;
    VkDevice* m_logicalDevice;

    WindowProperties* m_windowProperties;

    std::vector<VkImageView> m_swapChainImageViews;
  };
}

#endif // SWAPCHAIN_HPP