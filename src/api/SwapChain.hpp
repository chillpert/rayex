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
    void createFramebuffers(VkRenderPass* renderPass);

    void destroySwapChain();
    void destroyImageView();
    void destroyFramebuffers();

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static inline VkSurfaceKHR* getSurface() { return s_surface; }

    inline VkSwapchainKHR* getSwapChain() { return &m_swapChain; }
    inline VkFormat& getImageFormat() { return m_swapChainImageFormat; }
    inline VkExtent2D& getExtent() { return m_swapChainExtent; }

    inline std::vector<VkImageView>& getImageViews() { return m_swapChainImageViews; }
    inline std::vector<VkFramebuffer>& getFramebuffers() { return m_swapChainFramebuffers; }
    inline std::vector<VkImage>& getImages() { return m_swapChainImages; }

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
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
  };
}

#endif // SWAPCHAIN_HPP