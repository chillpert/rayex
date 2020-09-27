#ifndef API_HPP
#define API_HPP

#include "api/utility/DebugMessenger.hpp"
#include "api/Surface.hpp"
#include "api/Pipeline.hpp"
#include "api/RenderPass.hpp"
#include "api/Swapchain.hpp"
#include "api/image/Texture.hpp"
#include "api/misc/Vertex.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/StorageBuffer.hpp"
#include "api/descriptors/DescriptorSet.hpp"
#include "api/descriptors/DescriptorSetLayout.hpp"
#include "api/buffers/UniformBuffer.hpp"
#include "base/Gui.hpp"
#include "base/Base.hpp"
#include "api/raytracing/RayTracingBuilder.hpp"
#include "api/Descriptor.hpp"

namespace RAYEXEC_NAMESPACE
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
    Api( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera );

    /// @param window A pointer to a window object that the API will use to display an image.
    /// @param gui A pointer to a GUI object that will be rendered on top of the final image.
    /// @param camera A pointer to a camera object that will be used to "capture" the scene.
    Api( std::shared_ptr<Window> window, std::shared_ptr<Gui> gui, std::shared_ptr<Camera> camera );
    RX_API ~Api( );

    /// Used to set the GUI that will be used.
    /// 
    /// The GUI can be changed at runtime. This enables the client to swap between different pre-built GUIs on the fly.
    /// @param gui A pointer to a GUI object that will be rendered on top of the final image.
    /// @param initialize If true, the GUI object will be initialized (false if not specified).
    RX_API void setGui( const std::shared_ptr<Gui> gui, bool initialize = false );

    /// Initializes all API components.
    bool init( );

    /// Used to update and upload uniform buffers.
    void update( );

    /// Retrieves an image from the swapchain and presents it.
    bool render( );

    /// Used to add another arbitrary node to the scene.
    /// @param node A pointer to node to add.
    template <typename T = Model>
    void pushNode( const std::shared_ptr<Node> node, bool record = true )
    {
      if ( dynamic_cast<GeometryNode*>( node.get( ) ) )
      {
        auto ptr = std::dynamic_pointer_cast<GeometryNode>( node );

        auto it = models.find( ptr->modelPath );
        if ( it == models.end( ) )
          models.insert( { ptr->modelPath, std::make_shared<T>( ptr->modelPath ) } );

        geometryNodes.push_back( ptr );

        // Fill scene description buffer.
        ptr->rtInstance.transform = ptr->worldTransform;
        ptr->rtInstance.transformIT = glm::transpose( glm::inverse( ptr->worldTransform ) );
        
        std::cout << glm::to_string( ptr->rtInstance.transformIT ) << std::endl,

        this->rtInstances.push_back( ptr->rtInstance );
        this->uploadSceneDescriptionData = true;

        // Handle the node's texture.
        auto texturePaths = ptr->material.getTextures( );

        for ( const auto& texturePath : texturePaths )
        {
          auto it = textures.find( texturePath );
          // Texture does not exist already. It will be created.
          if ( it == textures.end( ) )
            textures.insert( { texturePath, std::make_shared<Texture>( texturePath ) } );
        }

        if ( record )
          initModel( ptr );
      }
      else if ( dynamic_cast<LightNode*>( node.get( ) ) )
      {
        if ( dynamic_cast<DirectionalLightNode*>( node.get( ) ) )
        {
          auto dirLightNodePtr = std::dynamic_pointer_cast<DirectionalLightNode>( node );
          dirLightNodes.push_back( dirLightNodePtr );

          ++totalDirectionalLights;
        }
        else if ( dynamic_cast<PointLightNode*>( node.get( ) ) )
        {
          auto pointLightNodePtr = std::dynamic_pointer_cast<PointLightNode>( node );
          pointLightNodes.push_back( pointLightNodePtr );

          ++totalPointLights;
        }
      }

      if ( record )
      {
        swapchainCommandBuffers.reset( );
        recordSwapchainCommandBuffers( );
      }
    }

    RX_API void popNode( const std::shared_ptr<Node> node );

    /// Used to overwrite the entire scene with new nodes.
    /// @param nodes A vector of pointers to nodes describing the new scene.
    template <typename T = Model>
    void setNodes( const std::vector<std::shared_ptr<Node>>& nodes )
    {
      geometryNodes.clear( );
      geometryNodes.reserve( g_maxGeometryNodes );

      for ( const auto& node : nodes )
        pushNode<T>( node );

      swapchainCommandBuffers.reset( );
      recordSwapchainCommandBuffers( );
    }

    /// Re-initializes the render pass to support the GUI and initializes the GUI itself.
    RX_API bool initGui( );

    Settings* settings = nullptr;

  private:
    void updateAccelerationStructure( );

    bool initPipelines( );

    /// Initializes the render pass with a color and depth attachment.
    bool initRenderPass( );

    /// Initializes the model provided by the node.
    /// 
    /// The model will be added to RAYEXEC_NAMESPACE::Api::models to make sure there are no duplicates.
    /// Similarily, all textures required by the model will be stored individualy inside RAYEXEC_NAMESPACE::Api::textures.
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
    void initDescriptorSets( );

    /// Recreates the swapchain and re-records the swapchain command buffers.
    void recreateSwapchain( );

    /// Acquires the next image from the swapchain.
    bool prepareFrame( );

    /// Submits the swapchain command buffers to a queue and presents an image on the screen.
    bool submitFrame( );

    std::shared_ptr<Window> window;
    std::shared_ptr<Camera> camera;

    // Destruction through RAII for following members:
    vk::UniqueInstance instance;
    DebugMessenger debugMessenger;
    Surface surface;
    vk::UniqueDevice device;
    vk::UniqueCommandPool graphicsCmdPool;
    vk::UniqueCommandPool transferCmdPool;
    std::vector<vk::UniqueFence> inFlightFences;
    std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
    std::vector<vk::UniqueSemaphore> finishedRenderSemaphores;

    // Descriptors for ray-tracing-related data ( no equivalent in rasterization shader ).
    DescriptorSetLayout rtDescriptorSetLayout;
    vk::UniqueDescriptorPool rtDescriptorPool;
    DescriptorSet rtDescriptorSets;

    // Descriptors for model-related data.
    DescriptorSetLayout rtModelDescriptorSetLayout; ///< @note Each RAYEXEC_NAMESPACE::Model has its own descriptor set.
    vk::UniqueDescriptorPool rtModelDescriptorPool;
    DescriptorSetLayout rsModelDescriptorSetLayout;
    vk::UniqueDescriptorPool rsModelDescriptorPool;

    // Descriptors for scene-related data.
    DescriptorSetLayout rtSceneDescriptorSetLayout;
    vk::UniqueDescriptorPool rtSceneDescriptorPool;
    DescriptorSet rtSceneDescriptorSets;
    DescriptorSetLayout rsSceneDescriptorSetLayout;
    vk::UniqueDescriptorPool rsSceneDescriptorPool;
    DescriptorSet rsSceneDescriptorSets;

    CameraUbo cameraUbo;
    UniformBuffer cameraUniformBuffer;
    UniformBuffer lightsUniformBuffer;
    
    std::vector<RayTracingInstance> rtInstances;
    StorageBuffer rayTracingInstancesBuffer;
    bool uploadSceneDescriptionData = false;

    // Nodes to render
    std::list<std::shared_ptr<GeometryNode>> geometryNodes;
    std::list<std::shared_ptr<DirectionalLightNode>> dirLightNodes;
    std::list<std::shared_ptr<PointLightNode>> pointLightNodes;
    // Models
    std::unordered_map<std::string, std::shared_ptr<Model>> models;
    // Textures
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

    Swapchain swapchain;
    RenderPass renderPass;
    Pipeline rsPipeline;
    Pipeline rtPipeline;
    CommandBuffer swapchainCommandBuffers;

    vk::Viewport viewport;
    vk::Rect2D scissor;
    
    std::shared_ptr<Gui> gui = nullptr;

    // No destruction necessary for following members:
    vk::PhysicalDevice physicalDevice;
    std::vector<vk::Fence> imagesInFlight;

    RayTracingBuilder rayTracingBuilder;

    bool needSwapchainRecreate = false;
  
    uint32_t totalDirectionalLights = 0;
    uint32_t totalPointLights = 0;
    bool reloadShader = false;
  };
}

#endif // API_HPP