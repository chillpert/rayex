#ifndef API_HPP
#define API_HPP

#include "Instance.hpp"
#include "DebugMessenger.hpp"
#include "Queues.hpp"
#include "Surface.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "Model.hpp"

#include "devices/PhysicalDevice.hpp"
#include "devices/Device.hpp"

#include "swapchain/Swapchain.hpp"
#include "swapchain/Images.hpp"
#include "swapchain/ImageViews.hpp"
#include "swapchain/Framebuffers.hpp"

#include "sync/Semaphore.hpp"
#include "sync/Fence.hpp"

#include "texture/Texture.hpp"

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
    CommandPool m_graphicsCmdPool;
    std::vector<Fence> m_inFlightFences;
    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_finishedRenderSemaphores;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    DescriptorSetLayout m_descriptorSetLayout;
    DescriptorPool m_descriptorPool;
    UniformBuffers m_uniformBuffers;
    // TODO: temporary
    Texture m_texture;
  
  public:
    std::vector<std::shared_ptr<Model>> m_models;

  private:
    Swapchain m_swapchain;
    ImageViews m_imageViews;
    RenderPass m_renderPass;
    Pipeline m_pipeline;
    CommandBuffers m_commandBuffers;
    Framebuffers m_framebuffers;

    // No destruction necessary for following members:
    Images m_images;
    PhysicalDevice m_physicalDevice;
    Queues m_queues;
    DescriptorSets m_descriptorSets;
    std::vector<VkFence> m_imagesInFlight;
  };
}

#endif // API_HPP