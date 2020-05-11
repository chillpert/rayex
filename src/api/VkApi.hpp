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
#include "Semaphore.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"

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

    std::shared_ptr<VkShader> m_vertexShader;
    std::shared_ptr<VkShader> m_fragmentShader;

    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    RenderPass renderPass;
    Pipeline pipeline;

    Semaphore imageAvailableSemaphore;
    Semaphore finishedRenderSemaphore;
    VkQueue m_queue;
    CommandPool commandPool;
    CommandBuffer commandBuffer;
  };
}

#endif // VK_API_HPP