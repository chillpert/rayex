#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "api/Surface.hpp"
#include "api/Queues.hpp"
#include "api/texture/ImageView.hpp"
#include "Framebuffer.hpp"
#include "api/texture/Image.hpp"

namespace RX
{
  struct SwapchainInfo
  {
    std::shared_ptr<Window> window;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;
    VkFormat surfaceFormat;
    VkFormat depthFormat;
    VkColorSpaceKHR surfaceColorSpace;
    VkPresentModeKHR surfacePresentMode;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<uint32_t> queueFamilyIndices;
    VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT;
    VkRenderPass renderPass;

    VkExtent2D extent; // Ignore, will be initialized automatically
    Image depthImage; // Ignore, will be initialized automatically
    ImageView depthImageView; // Ignore, will be initialized automatically
    std::vector<VkImage> images; // Ignore, will be initialized automatically
    std::vector<ImageView> imageViews; // Ignore, will be initialized automatically
    std::vector<Framebuffer> framebuffers; // Ignore, will be initialized automatically
  };

  class Swapchain
  {
  public:
    ~Swapchain();

    inline VkSwapchainKHR get() { return m_swapchain; }    
    inline SwapchainInfo& getInfo() { return m_info; }

    void initialize(SwapchainInfo& info);
    void destroy();

    void initDepthImage();
    void initDepthImageView();
    void initImageViews();
    void initFramebuffers();

    void destroyDepthImage();
    void destroyDepthImageView();
    void destroyImageViews();
    void destroyFramebuffers();

    void acquireNextImage(VkSemaphore semaphore, VkFence fence, uint32_t* imageIndex);
    
  private:
    VkSwapchainKHR m_swapchain;
    SwapchainInfo m_info;

    bool m_created = false;
  };

  VkFormat getSupportedDepthFormat(VkPhysicalDevice physicalDevice);
}

#endif // SWAPCHAIN_HPP