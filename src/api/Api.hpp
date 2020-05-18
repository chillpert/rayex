#ifndef API_HPP
#define API_HPP

#include "QueueManager.hpp"
#include "DebugMessenger.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"
#include "swapchain/Swapchain.hpp"
#include "swapchain/Images.hpp"
#include "swapchain/ImageViews.hpp"
#include "swapchain/Framebuffers.hpp"
#include "Surface.hpp"
#include "sync/Semaphore.hpp"
#include "CommandPool.hpp"
#include "CommandBuffers.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"

namespace RX
{
  class Api
  {
  public:
    Api(std::shared_ptr<Window> window);

    void initialize();
    bool update();
    bool render();
    void clean();
  
  private:
    std::shared_ptr<Window> m_window;

    // Destruction through RAII for following members:
    Instance m_instance;
    DebugMessenger m_debugMessenger;
    Surface m_surface;
    Device m_device;
    Swapchain m_swapchain;
    ImageViews m_imageViews;
    RenderPass m_renderPass;
    Pipeline m_pipeline;
    Framebuffers m_framebuffers;
    CommandPool m_commandPool;
    Semaphore m_finishedRenderSemaphore;
    Semaphore m_imageAvailableSemaphore;

    // No destruction necessary for following members:
    Images m_images;
    PhysicalDevice m_physicalDevice;
    QueueManager m_queueManager;
    CommandBuffers m_commandBuffers;
  };
}

#endif // API_HPP