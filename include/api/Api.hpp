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

namespace RENDERER_NAMESPACE
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
    template <typename T = Model>
    RX_API void pushNode( const std::shared_ptr<Node> node, bool record = true )
    {
      if ( dynamic_cast<GeometryNode*>( node.get( ) ) )
      {
        auto ptr = std::dynamic_pointer_cast<GeometryNode>( node );

        auto it = m_models.find( ptr->m_modelPath );
        if ( it == m_models.end( ) )
          m_models.insert( { ptr->m_modelPath, std::make_shared<T>( ptr->m_modelPath ) } );

        m_geometryNodes.push_back( ptr );

        // Handle the node's texture.
        auto texturePaths = ptr->m_material.getTextures( );

        for ( const auto& texturePath : texturePaths )
        {
          auto it = m_textures.find( texturePath );
          // Texture does not exist already. It will be created.
          if ( it == m_textures.end( ) )
          {
            m_textures.insert( { texturePath, std::make_shared<Texture>( texturePath ) } );
          }
        }

        if ( record )
          initModel( ptr );
      }
      else if ( dynamic_cast<LightNode*>( node.get( ) ) )
      {
        if ( dynamic_cast<DirectionalLightNode*>( node.get( ) ) )
        {
          auto dirLightNodePtr = std::dynamic_pointer_cast<DirectionalLightNode>( node );
          m_dirLightNodes.push_back( dirLightNodePtr );
        }
        else if ( dynamic_cast<PointLightNode*>( node.get( ) ) )
        {
          auto pointLightNodePtr = std::dynamic_pointer_cast<PointLightNode>( node );
          m_pointLightNodes.push_back( pointLightNodePtr );
        }
      }

      if ( record )
      {
        m_swapchainCommandBuffers.reset( );
        recordSwapchainCommandBuffers( );
      }
    }

    /// Used to overwrite the entire scene with new nodes.
    /// @param nodes A vector of pointers to nodes describing the new scene.
    template <typename T = Model>
    RX_API void setNodes( const std::vector<std::shared_ptr<Node>>& nodes )
    {
      m_geometryNodes.clear( );
      m_geometryNodes.reserve( g_maxGeometryNodes );

      for ( const auto& node : nodes )
        pushNode<T>( node );

      m_swapchainCommandBuffers.reset( );
      recordSwapchainCommandBuffers( );
    }

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
    RX_API void initModel( const std::shared_ptr<GeometryNode> node );

    /// Records commands to the swapchain command buffers that will be used for rendering.
    /// @todo Rasterization has been removed for now. Might want to re-add rasterization support with RT-compatible shaders again.
    RX_API void recordSwapchainCommandBuffers( );

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

    // Descriptors for ray-tracing-related data.
    DescriptorSetLayout m_rtDescriptorSetLayout;
    vk::UniqueDescriptorPool m_rtDescriptorPool;
    DescriptorSet m_rtDescriptorSets;

    // Descriptors for model-related data.
    DescriptorSetLayout m_modelDescriptorSetLayout; ///< @note Each rx::Model has its own descriptor set.
    vk::UniqueDescriptorPool m_modelDescriptorPool;

    // Descriptors for scene-related data.
    DescriptorSetLayout m_sceneDescriptorSetLayout;
    vk::UniqueDescriptorPool m_sceneDescriptorPool;
    DescriptorSet m_sceneDescriptorSets;

    CameraUbo m_cameraUbo;
    UniformBuffer m_cameraUniformBuffer;
    UniformBuffer m_lightsUniformBuffer;

    // Nodes to render.
    std::vector<std::shared_ptr<GeometryNode>> m_geometryNodes;
    std::vector<std::shared_ptr<DirectionalLightNode>> m_dirLightNodes;
    std::vector<std::shared_ptr<PointLightNode>> m_pointLightNodes;
    // Models
    std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
    // Textures
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

    Swapchain m_swapchain;
    RenderPass m_renderPass;
    Pipeline m_rtPipeline;
    CommandBuffer m_swapchainCommandBuffers;
    
    std::shared_ptr<GuiBase> m_gui = nullptr;

    // No destruction necessary for following members:
    PhysicalDevice m_physicalDevice;
    std::vector<vk::Fence> m_imagesInFlight;

    RayTracingBuilder m_rayTracingBuilder;

    bool m_recreateSwapchain = false;
  };
}

#endif // API_HPP