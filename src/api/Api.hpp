#ifndef API_HPP
#define API_HPP

#include "Instance.hpp"
#include "DebugMessenger.hpp"
#include "QueueManager.hpp"
#include "Surface.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Semaphore.hpp"
#include "Fence.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSetLayout.hpp"
#include "UniformBuffer.hpp"
#include "Gui.hpp"

namespace RX
{
  class Api
  {
  public:
    Api(std::shared_ptr<Window> window);
    RX_API ~Api();

    void initialize();
    bool update();
    bool render();
  
    void clearModels();
    void pushModel(const std::shared_ptr<Model> model);
    void setModels(const std::vector<std::shared_ptr<Model>>& models);

  private:
    void initInstance();
    void initDebugMessenger();
    void initSurface();
    void initPhysicalDevice();
    void initQueues();
    void initDevice();
    void initRenderPass();
    void initSwapchain();
    void initSwapchainImageViews();
    void initPipeline(bool firstRun = true);
    void initgraphicsCmdPool();
    void inittransferCmdPool();
    void initDepthBuffering();
    void initSwapchainFramebuffers();
    void initModels(bool firstRun = true);
    void initSwapchainCmdBuffers();
    void recordSwapchainCommandBuffers();

    void clean();
    void recreateSwapchain();

    void initRayTracing();

    std::shared_ptr<Window> m_window;

    // Destruction through RAII for following members:
    Instance m_instance;
    DebugMessenger m_debugMessenger;
    Surface m_surface;
    Device m_device;
    CommandPool m_graphicsCmdPool;
    CommandPool m_transferCmdPool;
    std::vector<Fence> m_inFlightFences;
    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_finishedRenderSemaphores;

    DescriptorSetLayout m_descriptorSetLayout;
    std::vector<std::shared_ptr<Model>> m_models;
    DescriptorPool m_descriptorPool;
    Swapchain m_swapchain;
    std::vector<ImageView> m_swapchainImageViews;
    RenderPass m_renderPass;
    Pipeline m_pipeline;
    CommandBuffer m_swapchainCmdBuffers;
    std::vector<Framebuffer> m_swapchainFramebuffers;
    Image m_depthImage;
    ImageView m_depthImageView;

    // No destruction necessary for following members:
    PhysicalDevice m_physicalDevice;
    QueueManager m_queueManager;
    std::vector<VkFence> m_imagesInFlight;

    VkPhysicalDeviceRayTracingPropertiesKHR m_rayTracingProperties;
  };
}

#endif // API_HPP