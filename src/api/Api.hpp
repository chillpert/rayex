#ifndef API_HPP
#define API_HPP

#include "Instance.hpp"
#include "DebugMessenger.hpp"
#include "Queues.hpp"
#include "Surface.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"

#include "devices/PhysicalDevice.hpp"
#include "devices/Device.hpp"

#include "swapchain/Swapchain.hpp"
#include "swapchain/Images.hpp"
#include "swapchain/ImageViews.hpp"
#include "swapchain/Framebuffers.hpp"

#include "sync/Semaphore.hpp"
#include "sync/Fence.hpp"

#include "buffers/CommandPool.hpp"
#include "buffers/CommandBuffers.hpp"
#include "buffers/VertexBuffer.hpp"
#include "buffers/IndexBuffer.hpp"
#include "buffers/DescriptorSets.hpp"
#include "buffers/DescriptorPool.hpp"
#include "buffers/DescriptorSetLayout.hpp"
#include "buffers/UniformBuffers.hpp"

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
    void recreateSwapchain();

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
    CommandPool m_graphicsCmdPool;
    std::vector<VkFence> m_imagesInFlight;
    std::vector<Fence> m_inFlightFences;
    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_finishedRenderSemaphores;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    DescriptorSetLayout m_descriptorSetLayout;
    DescriptorSets m_descriptorSets;
    DescriptorPool m_descriptorPool;
    UniformBuffers m_uniformBuffers;

    // No destruction necessary for following members:
    Images m_images;
    PhysicalDevice m_physicalDevice;
    Queues m_queues;
    CommandBuffers m_commandBuffers;
  };
}

#endif // API_HPP