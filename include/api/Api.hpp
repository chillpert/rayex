#pragma once

#include "api/Bindings.hpp"
#include "api/Components.hpp"
#include "api/PostProcessingRenderer.hpp"
#include "api/RenderPass.hpp"
#include "api/Surface.hpp"
#include "api/Swapchain.hpp"
#include "api/Vertex.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/buffers/StorageBuffer.hpp"
#include "api/buffers/UniformBuffer.hpp"
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

  /// Initializes and owns all Vulkan components and renders an image.
  /// @note All API components and resources are freed using scope-bound destruction.
  /// @warning Because of scope-bound destruction it is required to pay close attention to the order of resource allocations.
  /// @ingroup API
  class Api
  {
  public:
    friend Rayex;

    Api( ) = default;
    ~Api( );

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

    void initGui( );

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

    /// Updates both top and bottom level acceleration structures as well as their descriptor bindings.
    void updateAccelerationStructuresDescriptors( );

    /// Handles swapchain and pipeline recreations triggered by the user using setters provided in RAYEX_NAMESPACE::Settings.
    void updateSettings( );

    /// Recreates the swapchain and re-records the swapchain command buffers.
    void recreateSwapchain( );

    /// Acquires the next image from the swapchain.
    void prepareFrame( );

    /// Submits the swapchain command buffers to a queue and presents an image on the screen.
    void submitFrame( );

    std::shared_ptr<Window> _window = nullptr;
    std::shared_ptr<Camera> _camera = nullptr;
    vk::UniqueInstance _instance;
    DebugMessenger _debugMessenger;
    Surface _surface;
    vk::UniqueDevice _device;
    vk::UniqueCommandPool _graphicsCmdPool;
    vk::UniqueCommandPool _transferCmdPool;
    PathTracer _pathTracer;

    std::vector<vk::Fence> _imagesInFlight;
    std::vector<vk::UniqueFence> _inFlightFences;
    std::vector<vk::UniqueSemaphore> _imageAvailableSemaphores;
    std::vector<vk::UniqueSemaphore> _finishedRenderSemaphores;

    Descriptors _ptDescriptors;
    Descriptors _ptSceneDescriptors;
    Descriptors _geometryDescriptors;

    std::vector<vk::DescriptorSet> _ptDescriptorSets;
    std::vector<vk::DescriptorSet> _ptSceneDescriptorSets;
    std::vector<vk::DescriptorSet> _geometryDescriptorSets;
    std::vector<vk::DescriptorSet> _textureDescriptorSets;

    Cubemap _environmentMap;
    std::vector<vk::UniqueSampler> _immutableSamplers;
    std::vector<StorageBuffer<uint32_t>> _indexBuffers;
    std::vector<StorageBuffer<Vertex>> _vertexBuffers;
    std::vector<StorageBuffer<MeshSSBO>> _meshBuffers;
    std::vector<std::shared_ptr<Texture>> _textures;

    StorageBuffer<GeometryInstanceSSBO> _geometryInstancesBuffer;

    UniformBuffer _cameraUniformBuffer;

    Swapchain _swapchain;
    PostProcessingRenderer _postProcessingRenderer;

    CommandBuffer _swapchainCommandBuffers;

    std::shared_ptr<Gui> _gui = nullptr;

    Scene* _scene       = nullptr;
    Settings* _settings = nullptr;

    bool _needSwapchainRecreate = false;
    bool _pipelinesReady        = false;
  };
} // namespace RAYEX_NAMESPACE
