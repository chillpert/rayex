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
    std::shared_ptr<Window> window;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::SurfaceKHR surface;
    vk::Format surfaceFormat;
    vk::Format depthFormat;
    vk::ColorSpaceKHR surfaceColorSpace;
    vk::PresentModeKHR surfacePresentMode;
    vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<uint32_t> queueFamilyIndices;
    vk::ImageAspectFlags imageAspect = vk::ImageAspectFlagBits::eColor;
    vk::RenderPass renderPass;

    vk::Extent2D extent; // Ignore, will be initialized automatically
    std::vector<vk::Image> images; // Ignore, will be initialized automatically
  };

  class Swapchain
  {
  public:
    ~Swapchain();

    inline vk::SwapchainKHR get() { return m_swapchain; }    
    inline SwapchainInfo& getInfo() { return m_info; }

    void initialize(SwapchainInfo& info);
    void destroy();

    void acquireNextImage(vk::Semaphore semaphore, vk::Fence fence, uint32_t* imageIndex);
    
  private:
    vk::SwapchainKHR m_swapchain;
    SwapchainInfo m_info;

    bool m_created = false;
  };

  vk::Format getSupportedDepthFormat(vk::PhysicalDevice physicalDevice);
}

#endif // SWAPCHAIN_HPP