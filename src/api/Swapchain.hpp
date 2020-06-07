#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "Surface.hpp"
#include "QueueManager.hpp"
#include "ImageView.hpp"
#include "Framebuffer.hpp"
#include "Image.hpp"

namespace RX
{
  struct SwapchainInfo
  {
    Window* window;
    Surface* surface;

    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    std::vector<uint32_t> queueFamilyIndices;
    vk::ImageAspectFlags imageAspect;
    vk::RenderPass renderPass;
    vk::Extent2D customExtent; // Optional, leave empty if you want to use the entire size available. // TODO: implement
  };

  class Swapchain
  {
  public:
    ~Swapchain();

    inline vk::SwapchainKHR get() { return m_swapchain; }    
    inline SwapchainInfo& getInfo() { return m_info; }

    inline vk::Extent2D getExtent() const { return m_extent; }
    inline vk::Image& getImage(size_t index) { return m_images[index]; }
    inline std::vector<vk::Image>& getImages() { return m_images; }

    void initialize(SwapchainInfo& info);
    void destroy();

    void acquireNextImage(vk::Semaphore semaphore, vk::Fence fence, uint32_t* imageIndex);
    
  private:
    vk::SwapchainKHR m_swapchain;
    SwapchainInfo m_info;

    vk::Extent2D m_extent;
    std::vector<vk::Image> m_images;
  };

  vk::Format getSupportedDepthFormat(vk::PhysicalDevice physicalDevice);
}

#endif // SWAPCHAIN_HPP