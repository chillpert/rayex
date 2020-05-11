#ifndef VK_API_HPP
#define VK_API_HPP

#include "window/Window.hpp"

#include "VkShader.hpp"
#include "VkMessenger.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Surface.hpp"

namespace RX
{
  class VkApi
  {
  public:
    VkApi(std::shared_ptr<Window> window);

    void initialize();
    bool update();
    bool render();
    void clean();
  
  private:
    std::shared_ptr<Window> m_window;

    Instance instance;
    VkMessenger m_messenger;
    PhysicalDevice physicalDevice;
    Device device;
    Swapchain swapchain;

    Surface surface;

    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    VkFormat m_swapChainFormat;

    VkRenderPass m_renderPass;
    VkPipeline m_pipeline;
    std::shared_ptr<VkShader> m_vertexShader;
    std::shared_ptr<VkShader> m_fragmentShader;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_finishedRenderSemaphore;
    VkQueue m_queue;
    VkCommandPool m_commandPool;
    VkCommandBuffer m_commandBuffer;
  };
}

#endif // VK_API_HPP