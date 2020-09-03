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
#include "RayTracingBuilder.hpp"

namespace rx
{
  /// Initializes and owns all Vulkan components and displays a picture on the screen.
  /// @note All Vulkan components and resources are freed using scope-bound destruction.
  class Api
  {
  public:
    /// @param window A pointer to a window object that the API will use to display an image.
    /// @param camera A pointer to a camera object that will be used to "capture" the scene.
    Api( std::shared_ptr<WindowBase> window, std::shared_ptr<CameraBase> camera );

    /// @param window A pointer to a window object that the API will use to display an image.
    /// @param gui A pointer to a GUI object that will be rendered on top of the final image.
    /// @param camera A pointer to a camera object that will be used to "capture" the scene.
    Api( std::shared_ptr<WindowBase> window, std::shared_ptr<GuiBase> gui, std::shared_ptr<CameraBase> camera );
    RX_API ~Api( );

    /// Used to set the GUI that will be used.
    RX_API void setGui( const std::shared_ptr<GuiBase> gui );

    void init( );
    bool update( );
    bool render( );

    void pushNode( const std::shared_ptr<Node> nodes, bool record = true );
    void setNodes( const std::vector<std::shared_ptr<Node>>& nodes );
    RX_API void initGui( );

  private:
    void initRenderPass( );
    void initModel( const std::shared_ptr<GeometryNode> node );

    void recordSwapchainCommandBuffers( );
    void initSyncObjects( );

    void initDescriptorSetLayouts( );

    void clean( );
    void recreateSwapchain( );

    bool prepareFrame( );
    bool submitFrame( );

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

    DescriptorSetLayout m_rtDescriptorSetLayout;
    vk::UniqueDescriptorPool m_rtDescriptorPool;
    DescriptorSet m_rtDescriptorSets;

    DescriptorSetLayout m_descriptorSetLayout;
    vk::UniqueDescriptorPool m_descriptorPool;

    UniformBuffer m_cameraUniformBuffer;

    // Nodes to render.
    std::vector<std::shared_ptr<GeometryNode>> m_geometryNodes;
    std::vector<std::shared_ptr<LightNode>> m_lightNodes;
    // Models
    std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
    // Textures
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

    Swapchain m_swapchain;
    RenderPass m_renderPass;
    Pipeline m_rtPipeline;
    CommandBuffer m_swapchainCommandBuffers;
    
    std::shared_ptr<GuiBase> m_gui;

    // No destruction necessary for following members:
    PhysicalDevice m_physicalDevice;
    Queues m_queues;
    std::vector<vk::Fence> m_imagesInFlight;

    RayTracingBuilder m_rayTracingBuilder;

    bool m_recreateSwapchain = false;
  };
}

#endif // API_HPP