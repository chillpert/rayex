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
  /// @note All API components and resources are freed using scope-bound destruction.
  /// @warning Because of scope-bound destruction it is required to pay close attention to the order of resource allocations done by the client.
  /// @ingroup API
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
    /// 
    /// The GUI can be changed at runtime. This enables the client to swap between different pre-built GUIs on the fly.
    /// @param gui A pointer to a GUI object that will be rendered on top of the final image.
    /// @param initialize If true, the GUI object will be initialized (false if not specified).
    RX_API void setGui( const std::shared_ptr<GuiBase> gui, bool initialize = false );

    /// Initializes all API components.
    void init( );

    /// Used to update and upload uniform buffers.
    bool update( );

    /// Retrieves an image from the swapchain and presents it.
    bool render( );

    /// Used to add another arbitrary node to the scene.
    /// @param node A pointer to node to add.
    void pushNode( const std::shared_ptr<Node> nodes, bool record = true );

    /// Used to overwrite the entire scene with new nodes.
    /// @param nodes A vector of pointers to nodes describing the new scene.
    void setNodes( const std::vector<std::shared_ptr<Node>>& nodes );

    /// Re-initializes the render pass to support the GUI and initializes the GUI itself.
    RX_API void initGui( );

  private:
    /// Initializes the render pass with a color and depth attachment.
    void initRenderPass( );

    /// Initializes the model provided by the node.
    /// 
    /// The model will be added to rx::Api::m_models to make sure there are no duplicates.
    /// Similarily, all textures required by the model will be stored individualy inside rx::Api::m_textures.
    /// If a model or a texture are already known to the application and have been initialized, they will be re-used instead of being initialized.
    /// @param node A pointer to a geometry node.
    /// @todo The function currently recreates the entire TLAS and BLAS everytime a model is added, which is very inefficient.
    void initModel( const std::shared_ptr<GeometryNode> node );

    /// Records commands to the swapchain command buffers that will be used for rendering.
    /// @todo Rasterization has been removed for now. Might want to re-add rasterization support with RT-compatible shaders again.
    void recordSwapchainCommandBuffers( );

    /// Creates all fences and semaphores required to sync the rendering process.
    void initSyncObjects( );

    /// Creates a descriptor set layout for each the ray tracing components and the models.
    void initDescriptorSetLayouts( );

    /// Recreates the swapchain and re-records the swapchain command buffers.
    void recreateSwapchain( );

    /// Acquires the next image from the swapchain.
    bool prepareFrame( );

    /// Submits the swapchain command buffers to a queue and presents an image on the screen.
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