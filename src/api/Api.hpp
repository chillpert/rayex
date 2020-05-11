#ifndef API_HPP
#define API_HPP

#include "window/Window.hpp"

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

    Instance instance;
    DebugMessenger debugMessenger;
    PhysicalDevice physicalDevice;
    Device device;
    Swapchain swapchain;
    Surface surface;

    Shader vs;
    Shader fs;

    RenderPass renderPass;
    Pipeline pipeline;

    Semaphore imageAvailableSemaphore;
    Semaphore finishedRenderSemaphore;
    VkQueue m_queue;
    CommandPool commandPool;
    CommandBuffer commandBuffer;
  };
}

#endif // API_HPP