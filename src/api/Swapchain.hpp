#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "Surface.hpp"
#include "QueueManager.hpp"

namespace RX
{
  class Swapchain
  {
  public:
    inline VkSwapchainKHR get() { return swapchain; }
    
    inline std::vector<VkImage>& getImages() { return images; }
    inline std::vector<VkImageView>& getImageViews() { return imageViews; }
    inline std::vector<VkFramebuffer> getFramebuffers() { return framebuffers; }
    
    inline VkExtent2D& getExtent() { return extent; } 

    void create(VkPhysicalDevice physicalDevice, VkDevice device, Surface surface, std::shared_ptr<Window> window, QueueManager& queueManager);
    void destroy(VkDevice device);
    
    void createImages(VkDevice device);
    void createImageViews(VkDevice device, Surface surface);
    void createFramebuffers(VkDevice device, VkRenderPass renderPass, std::shared_ptr<Window> window);

  private:
    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;

    VkExtent2D extent;
  };
}

#endif // SWAPCHAIN_HPP