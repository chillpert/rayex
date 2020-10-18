#ifndef API_HPP
#define API_HPP

#include "api/Bindings.hpp"
#include "api/Pipeline.hpp"
#include "api/RenderPass.hpp"
#include "api/Scene.hpp"
#include "api/Surface.hpp"
#include "api/Swapchain.hpp"
#include "api/buffers/CommandBuffer.hpp"
#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/StorageBuffer.hpp"
#include "api/buffers/UniformBuffer.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/image/Texture.hpp"
#include "api/misc/Components.hpp"
#include "api/misc/Vertex.hpp"
#include "api/raytracing/RayTracingBuilder.hpp"
#include "api/utility/DebugMessenger.hpp"
#include "api/utility/Util.hpp"
#include "base/Gui.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Initializes and owns all Vulkan components and displays a picture on the screen.
  /// @note All API components and resources are freed using scope-bound destruction.
  /// @warning Because of scope-bound destruction it is required to pay close attention to the order of resource allocations done by the user.
  /// @ingroup API
  class Api
  {
  public:
    friend RayExec;

    Api( ) = default;
    RX_API ~Api( );

  private:
    /// @param window A pointer to a window object that the API will use to display an image.
    /// @param camera A pointer to a camera object that will be used to "capture" the scene.
    Api( std::shared_ptr<Window> window, std::shared_ptr<Camera> camera );

    Api( const Api& )  = delete;
    Api( const Api&& ) = delete;
    auto operator=( const Api& ) -> Api& = delete;
    auto operator=( const Api && ) -> Api& = delete;

    /// Used to set the GUI that will be used.
    ///
    /// The GUI can be changed at runtime. This enables the user to swap between different pre-built GUIs on the fly.
    /// @param gui A pointer to a GUI object that will be rendered on top of the final image.
    /// @param initialize If true, the GUI object will be initialized (false if not specified).
    RX_API void setGui( const std::shared_ptr<Gui>& gui, bool initialize = false );

    /// Initializes all universal Vulkan objects.
    void initBase( );

    /// Initializes all Vulkan objects that are scene-specific.
    void initScene( );

    /// Used to update and upload uniform buffers.
    void update( );

    /// Retrieves an image from the swapchain and presents it.
    auto render( ) -> bool;

    /// Re-initializes the render pass to support the GUI and initializes the GUI itself.
    RX_API void initGui( );

    /// Updates both top and bottom level acceleration structures as well as their descriptor bindings.
    RX_API void updateAccelerationStructures( );

    /// Initilializes a rasterization as well as a graphics pipeline.
    /// @todo This function will be pointless once the new pipeline system is implemented.
    void initPipelines( );

    /// Initializes the render pass with a color and depth attachment.
    void initRenderPass( );

    /// Records commands to the swapchain command buffers that will be used for rendering.
    /// @todo Rasterization has been removed for now. Might want to re-add rasterization support with RT-compatible shaders again.
    RX_API void recordSwapchainCommandBuffers( );

    /// Creates all fences and semaphores required to sync the rendering process.
    void initSyncObjects( );

    /// Creates a descriptor set layout for each the ray tracing components and the models.
    void initDescriptorSets( );

    void initModelBuffers( );

    /// Fills a buffer with the ray tracing instances.
    void initRayTracingInstancesBuffer( );

    /// Updates scene descriptors for camera, lights and ray tracing instances.
    void updateSceneDescriptors( );

    /// Updates the descriptor set with bindings to the total vertices and total indices buffers.
    void updateRayTracingModelData( );

    /// Handles swapchain and pipeline recreations triggered by the user using setters provided in RAYEXEC_NAMESPACE::Settings.
    void updateSettings( );

    /// Updates all uniform buffers light light sources.
    void updateUniformBuffers( );

    /// Recreates the swapchain and re-records the swapchain command buffers.
    void recreateSwapchain( );

    /// Acquires the next image from the swapchain.
    auto prepareFrame( ) -> bool;

    /// Submits the swapchain command buffers to a queue and presents an image on the screen.
    auto submitFrame( ) -> bool;

    /// Records rasterization draw calls to the swapchain command buffers.
    void rasterize( );

    /// Records ray tracing calls to the swapchain command buffers.
    void rayTrace( );

    RX_API std::shared_ptr<Geometry> findGeometry( uint32_t geometryIndex );

    // Destruction through RAII for following members:
    std::shared_ptr<Window> window = nullptr;                  ///< A pointer to a RAYEXEC_NAMESPACE::Window object whose surface is going to be rendered to.
    std::shared_ptr<Camera> camera = nullptr;                  ///< A pointer to a RAYEXEC_NAMESPACE::Camera object whose matrices will be used for rendering.
    vk::UniqueInstance instance;                               ///< A unique Vulkan instance.
    DebugMessenger debugMessenger;                             ///< The debug messenger for logging validation messages.
    Surface surface;                                           ///< The surface containing surface parameters.
    vk::UniqueDevice device;                                   ///< A unique Vulkan logical device for interfacing the physical device.
    vk::UniqueCommandPool graphicsCmdPool;                     ///< A single unique Vulkan command pool for graphics queue operations.
    vk::UniqueCommandPool transferCmdPool;                     ///< A single unique Vulkan command pool for transfer queue operations.
    std::vector<vk::UniqueFence> inFlightFences;               ///< In flight fences used for synchronization.
    std::vector<vk::UniqueSemaphore> imageAvailableSemaphores; ///< All semaphores for signaling that a particular swapchain image is available.
    std::vector<vk::UniqueSemaphore> finishedRenderSemaphores; ///< All semaphores for signaling that a render operation has finished.

    Descriptors rtDescriptors;                       ///< Descriptors for ray-tracing-related data ( no equivalent in rasterization shader ).
    std::vector<vk::DescriptorSet> rtDescriptorSets; ///< A vector of Vulkan descriptor sets for ray tracing-related data.

    Descriptors rtSceneDescriptors;                       ///< Descriptors for scene-related data in ray tracing.
    std::vector<vk::DescriptorSet> rtSceneDescriptorSets; ///< A vector of Vulkan descriptor sets for scene-related data in rasterization shaders.

    Descriptors rsSceneDescriptors;                       ///< Descriptors for scene-related data in rasterization.
    std::vector<vk::DescriptorSet> rsSceneDescriptorSets; ///< A vector of Vulkan descriptor sets for scene-related data in ray tracing shaders.

    Descriptors vertexDataDescriptors;                           ///< Descriptors for vertex data.
    std::vector<vk::DescriptorSet> vertexDataDescriptorSets;     ///< A vector of Vulkan descriptor sets for vertex data.
    std::vector<vk::DescriptorBufferInfo> vertexDataBufferInfos; ///< A vector of Vulkan descriptor buffer infos for the vertex data.

    Descriptors indexDataDescriptors;                           ///< Descriptors for index data.
    std::vector<vk::DescriptorSet> indexDataDescriptorSets;     ///< A vector of Vulkan descriptor sets for index data.
    std::vector<vk::DescriptorBufferInfo> indexDataBufferInfos; ///< A vector of Vulkan descriptor buffer infos for the index data.

    CameraUbo cameraUbo;               ///< A uniform buffer object containing camera data.
    UniformBuffer cameraUniformBuffer; ///< A uniform buffer containing camera data.
    UniformBuffer lightsUniformBuffer; ///< A uniform buffer containing lights data.

    std::vector<vk::DescriptorBufferInfo> meshDataBufferInfos;

    StorageBuffer rayTracingInstancesBuffer;        ///< A storage buffer for the ray tracing instances.
    bool uploadRayTracingInstancesToBuffer = false; ///< Keeps track of whether or not to upload the ray tracing instances to their respective buffer the next time update() is called.

    Swapchain swapchain;   ///< A RAYEXEC_NAMESPACE::Swapchain to manage swapchain related operations.
    RenderPass renderPass; ///< A RAYEXEC_NAMESPACE::RenderPass to create, begin and end a render pass.

    Pipeline rsPipeline; ///< A RAYEXEC_NAMESPACE::Pipeline for managing a rasterization pipeline.
    Pipeline rtPipeline; ///< A RAYEXEC_NAMESPACE::Pipeline for managing a ray tracing pipeline.

    std::vector<vk::UniquePipeline> rsPipelines;
    std::vector<vk::UniquePipeline> rtPipelines;

    CommandBuffer swapchainCommandBuffers; ///< A RAYEXEC_NAMESPACE::CommandBuffer containing as many command buffers as there are images in the swapchain.

    vk::Viewport viewport; ///< The application's viewport.
    vk::Rect2D scissor;    ///< The application's scissor.

    std::shared_ptr<Gui> gui = nullptr; ///< A pointer to a RAYEXEC_NAMESPACE::Gui object that will be used for rendering the GUI.

    // No destruction necessary for following members:
    Settings* settings = nullptr;                                                   ///< A pointer to a RAYEXEC_NAMESPACE::Settings object stored inside RAYEXEC_NAMESPACE::RayExec.
    std::vector<vk::GraphicsPipelineCreateInfo> graphicsPipelineCreateInfos;        ///<
    std::vector<vk::RayTracingPipelineCreateInfoKHR> rayTracingPipelineCreateInfos; ///<
    vk::PhysicalDevice physicalDevice;                                              ///< Stores the physical device (GPU).
    std::vector<vk::Fence> imagesInFlight;                                          ///< A vector of fences for synchronizing images that are in flight.

    RayTracingBuilder rayTracingBuilder; ///< The RAYEXEC_NAMESPACE::RayTracingBuilder for setting up all ray tracing-related structures and the ray tracing process itself.

    /// @todo Should be replaced with Settings:: ...
    bool needSwapchainRecreate = false;
    bool pipelinesReady        = false; ///< Keeps track of whether or not the graphics pipelines are ready.

    Scene scene;

    std::vector<VertexBuffer> vertexBuffers;
    std::vector<IndexBuffer> indexBuffers;
    std::vector<StorageBuffer> meshBuffers;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // API_HPP
