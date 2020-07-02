#ifndef API_HPP
#define API_HPP

#include "Instance.hpp"
#include "DebugMessenger.hpp"
#include "Queues.hpp"
#include "Surface.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "CommandBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "UniformBuffer.hpp"
#include "GuiBase.hpp"
#include "Base.hpp"
#include "RaytraceBuilder.hpp"

namespace rx
{
  class Api
  {
  public:
    Api( std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera );
    Api( std::shared_ptr<WindowBase> window, std::shared_ptr<GuiBase> gui, std::shared_ptr<CameraBase> camera );
    RX_API ~Api();

    void init();
    bool update();
    bool render();

    void pushNode( const std::shared_ptr<GeometryNodeBase> nodes, bool record = true );
    void setNodes( const std::vector<std::shared_ptr<GeometryNodeBase>>& nodes );

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
    void initPipeline( bool firstRun = true );
    void initGraphicsCommandPool();
    void initTransferCommandPool();
    void initDepthBuffering();
    void initSwapchainFramebuffers();
    void initDescriptorPool();
    void initModels( bool isNew );
    void initModel( const std::shared_ptr<GeometryNodeBase> node );
    void initSwapchainCommandBuffers();
    void initGui();
    void recordSwapchainCommandBuffers();

    void clean();
    void recreateSwapchain();

    void initRayTracing();

    std::shared_ptr<WindowBase> m_window;
    std::shared_ptr<CameraBase> m_camera;

    // Destruction through RAII for following members:
    Instance m_instance;
    DebugMessenger m_debugMessenger;
    Surface m_surface;
    Device m_device;
    vk::UniqueCommandPool m_graphicsCmdPool;
    vk::UniqueCommandPool m_transferCmdPool;
    std::vector<vk::UniqueFence> m_inFlightFences;
    std::vector<vk::UniqueSemaphore> m_imageAvailableSemaphores;
    std::vector<vk::UniqueSemaphore> m_finishedRenderSemaphores;

    DescriptorSetLayout m_descriptorSetLayout;

    // Nodes to render.
    std::vector<std::shared_ptr<GeometryNodeBase>> m_nodes;
    // Models
    std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
    // Textures
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

    vk::UniqueDescriptorPool m_descriptorPool;
    Swapchain m_swapchain;
    std::vector<vk::UniqueImageView> m_swapchainImageViews;
    RenderPass m_renderPass;
    RasterizationPipeline m_pipeline;
    CommandBuffer m_swapchainCommandBuffers;
    std::vector<vk::UniqueFramebuffer> m_swapchainFramebuffers;
    Image m_depthImage;
    vk::UniqueImageView m_depthImageView;

  public:
    std::shared_ptr<GuiBase> m_gui;
  private:
    // No destruction necessary for following members:
    PhysicalDevice m_physicalDevice;
    Queues m_queues;
    std::vector<vk::Fence> m_imagesInFlight;

    RaytraceBuilder m_raytraceBuilder;

    bool m_recreateSwapchain = false;
  };
}

#endif // API_HPP