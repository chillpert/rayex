#pragma once

#include "api/Bindings.hpp"
#include "api/Components.hpp"
#include "api/PostProcessingRenderer.hpp"
#include "api/RenderPass.hpp"
#include "api/Surface.hpp"
#include "api/Swapchain.hpp"
#include "api/Vertex.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/StorageBuffer.hpp"
#include "api/buffers/UniformBuffer.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/image/Cubemap.hpp"
#include "api/image/Texture.hpp"
#include "api/pathtrace/PathTracer.hpp"
#include "api/utility/DebugMessenger.hpp"
#include "api/utility/Util.hpp"
#include "base/Gui.hpp"
#include "base/Scene.hpp"

namespace RAYEX_NAMESPACE
{
  class Rayex;

  /// Initializes and owns all Vulkan components and displays a picture on the screen.
  /// @note All API components and resources are freed using scope-bound destruction.
  /// @warning Because of scope-bound destruction it is required to pay close attention to the order of resource allocations done by the user.
  /// @ingroup API
  class Api
  {
  public:
    friend Rayex;

    Api( ) = default;
    ~Api( );

    /// @param window A pointer to a window object that the API will use to display an image.
    /// @param camera A pointer to a camera object that will be used to "capture" the scene.
    Api( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera );

    Api( const Api& )  = delete;
    Api( const Api&& ) = delete;
    auto operator=( const Api& ) -> Api& = delete;
    auto operator=( const Api&& ) -> Api& = delete;

  private:
    /// Used to set the GUI that will be used.
    ///
    /// The GUI can be changed at runtime. This enables the user to swap between different pre-built GUIs on the fly.
    /// @param gui A pointer to a GUI object that will be rendered on top of the final image.
    /// @param initialize If true, the GUI object will be initialized (false if not specified).
    void setGui( const std::shared_ptr<Gui>& gui, bool initialize = false );

    /// Initializes all universal Vulkan objects.
    void initBase( );

    /// Initializes all Vulkan objects that are scene-specific.
    void initScene( );

    /// Used to update and upload uniform buffers.
    void update( );

    /// Retrieves an image from the swapchain and presents it.
    auto render( ) -> bool;

    /// Re-initializes the render pass to support the GUI and initializes the GUI itself.
    void initGui( );

    /// Updates both top and bottom level acceleration structures as well as their descriptor bindings.
    void updateAccelerationStructuresDescriptors( );

    /// Initilializes a rasterization as well as a graphics pipeline.
    /// @todo This function will be pointless once the new pipeline system is implemented.
    void initPipelines( );

    /// Records commands to the swapchain command buffers that will be used for rendering.
    /// @todo Rasterization has been removed for now. Might want to re-add rasterization support with RT-compatible shaders again.
    void recordSwapchainCommandBuffers( );

    /// Creates all fences and semaphores required to sync the rendering process.
    void initSyncObjects( );

    /// Creates a descriptor set layout for each the path tracing components and the models.
    void initDescriptorSets( );

    /// Updates scene descriptors for camera, lights and path tracing instances.
    void updateSceneDescriptors( );

    /// Updates the descriptor set with bindings to the total vertices and total indices buffers.
    void updateGeometryDescriptors( );

    /// Handles swapchain and pipeline recreations triggered by the user using setters provided in RAYEX_NAMESPACE::Settings.
    void updateSettings( );

    /// Recreates the swapchain and re-records the swapchain command buffers.
    void recreateSwapchain( );

    /// Acquires the next image from the swapchain.
    void prepareFrame( );

    /// Submits the swapchain command buffers to a queue and presents an image on the screen.
    void submitFrame( );

    std::shared_ptr<Window> _window = nullptr; ///< A pointer to a RAYEX_NAMESPACE::Window object whose surface is going to be rendered to.
    std::shared_ptr<Camera> _camera = nullptr; ///< A pointer to a RAYEX_NAMESPACE::Camera object whose matrices will be used for rendering.
    vk::UniqueInstance _instance;              ///< A unique Vulkan instance.
    DebugMessenger _debugMessenger;            ///< The debug messenger for logging validation messages.
    Surface _surface;                          ///< The surface containing surface parameters.
    vk::UniqueDevice _device;                  ///< A unique Vulkan logical device for interfacing the physical device.
    vk::UniqueCommandPool _graphicsCmdPool;    ///< A single unique Vulkan command pool for graphics queue operations.
    vk::UniqueCommandPool _transferCmdPool;    ///< A single unique Vulkan command pool for transfer queue operations.
    PathTracer _pathTracer;                    ///< The RAYEX_NAMESPACE::PathTracer for setting up all path tracing-related structures and the path tracing process itself.

    std::vector<vk::Fence> _imagesInFlight;                     ///< A vector of fences for synchronizing images that are in flight.
    std::vector<vk::UniqueFence> _inFlightFences;               ///< In flight fences used for synchronization.
    std::vector<vk::UniqueSemaphore> _imageAvailableSemaphores; ///< All semaphores for signaling that a particular swapchain image is available.
    std::vector<vk::UniqueSemaphore> _finishedRenderSemaphores; ///< All semaphores for signaling that a render operation has finished.

    Descriptors _ptDescriptors;       ///< Descriptors for ray-tracing-related data ( no equivalent in rasterization shader ).
    Descriptors _ptSceneDescriptors;  ///< Descriptors for scene-related data in path tracing.
    Descriptors _geometryDescriptors; ///< Descriptors for vertex data.

    std::vector<vk::DescriptorSet> _ptDescriptorSets;       ///< Descriptor sets for path tracing-related data.
    std::vector<vk::DescriptorSet> _ptSceneDescriptorSets;  ///< Descriptor sets for scene-related data in rasterization shaders.
    std::vector<vk::DescriptorSet> _geometryDescriptorSets; ///< Descriptor sets for vertex data.
    std::vector<vk::DescriptorSet> _textureDescriptorSets;  ///< Descriptor sets for texture data.

    Cubemap _environmentMap;
    std::vector<vk::UniqueSampler> _immutableSamplers;

    std::vector<VertexBuffer> _vertexBuffers;          ///< Multiple buffers for vertices because of variable descriptor indexing.
    std::vector<IndexBuffer> _indexBuffers;            ///< Multiple buffers for indices because of variable descriptor indexing.
    std::vector<StorageBuffer<MeshSSBO>> _meshBuffers; ///< Multiple buffers for meshes because of variable descriptor indexing.
    std::vector<std::shared_ptr<Texture>> _textures;   ///< Stores all textures.

    StorageBuffer<GeometryInstanceSSBO> _geometryInstancesBuffer; ///< A storage buffer for the path tracing instances.

    UniformBuffer _cameraUniformBuffer; ///< Uniform buffers containing camera data.

    Swapchain _swapchain; ///< A RAYEX_NAMESPACE::Swapchain to manage swapchain related operations.
    PostProcessingRenderer _postProcessingRenderer;

    CommandBuffer _swapchainCommandBuffers; ///< A RAYEX_NAMESPACE::CommandBuffer containing as many command buffers as there are images in the swapchain.

    std::shared_ptr<Gui> _gui = nullptr; ///< A pointer to a RAYEX_NAMESPACE::Gui object that will be used for rendering the GUI.

    bool _needSwapchainRecreate = false;   ///< Keeps track of whether or not the swapchain needs to be re-created.
    bool _pipelinesReady        = false;   ///< Keeps track of whether or not the graphics pipelines are ready.
    Settings* _settings         = nullptr; ///< Refers to the rendering settings stored in Rayex::settings.
    Scene* _scene               = nullptr; ///< Refers to the scene stored in Rayex::scene.
  };
} // namespace RAYEX_NAMESPACE
