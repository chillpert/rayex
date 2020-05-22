#ifndef API_HPP
#define API_HPP

#include "Queues.hpp"
#include "DebugMessenger.hpp"
#include "Instance.hpp"
#include "devices/PhysicalDevice.hpp"
#include "devices/Device.hpp"
#include "swapchain/Swapchain.hpp"
#include "swapchain/Images.hpp"
#include "swapchain/ImageViews.hpp"
#include "swapchain/Framebuffers.hpp"
#include "Surface.hpp"
#include "sync/Semaphore.hpp"
#include "sync/Fence.hpp"
#include "CommandPool.hpp"
#include "CommandBuffers.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "Buffers/VertexBuffer.hpp"

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
    void cleanSwapchain();
    void recreateSwapchain();

    std::shared_ptr<Window> m_window;

    // Destruction through RAII for following members:
    Instance m_instance;
    DebugMessenger m_debugMessenger;
    Surface m_surface;
    Device m_device;
    VertexBuffer m_vertexBuffer;
    Swapchain m_swapchain;
    ImageViews m_imageViews;
    RenderPass m_renderPass;
    Pipeline m_pipeline;
    Framebuffers m_framebuffers;
    CommandPool m_commandPool;
    //CommandPool m_transferCommandPool;

    std::vector<VkFence> m_imagesInFlight;
    std::vector<Fence> m_inFlightFences;
    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_finishedRenderSemaphores;

    // No destruction necessary for following members:
    Images m_images;
    PhysicalDevice m_physicalDevice;
    Queues m_queues;
    CommandBuffers m_commandBuffers;

    // TODO: make abstraction
    ;
  };
}

#endif // API_HPP