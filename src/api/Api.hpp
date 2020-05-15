#ifndef API_HPP
#define API_HPP

#include "window/Window.hpp"

#include "QueueManager.hpp"
#include "DebugMessenger.hpp"
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

    Instance m_instance;
    DebugMessenger m_debugMessenger;
    PhysicalDevice m_physicalDevice;
    Device m_device;
    Swapchain m_swapchain;
    Surface m_surface;

    RenderPass m_renderPass;
    Pipeline m_pipeline;

    QueueManager m_queueManager;

    Semaphore m_imageAvailableSemaphore;
    Semaphore m_finishedRenderSemaphore;

    CommandPool m_commandPool;
    CommandBuffer m_commandBuffer;
  };
}

#endif // API_HPP