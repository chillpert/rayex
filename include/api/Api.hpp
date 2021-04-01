#pragma once

#include "api/PostProcessingRenderer.hpp"
#include "api/Vertex.hpp"
#include "api/pathtrace/PathTracer.hpp"
#include "base/Gui.hpp"
#include "base/Scene.hpp"
#include "base/Window.hpp"

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
    RX_API ~Api( );

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

    void init( );

    /// Used to update and upload uniform buffers.
    void update( );

    /// Retrieves an image from the swapchain and presents it.
    void render( );

    void initGui( );

    void initPipelines( );

    /// Records commands to the swapchain command buffers that will be used for rendering.
    /// @todo Rasterization has been removed for now. Might want to re-add rasterization support with RT-compatible shaders again.
    void recordSwapchainCommandBuffers( );

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
    vkCore::DebugMessenger _debugMessenger;
    vkCore::Surface _surface;
    vk::UniqueDevice _device;
    vk::UniqueCommandPool _graphicsCmdPool;
    vk::UniqueCommandPool _transferCmdPool;

    PathTracer _pathTracer;
    PostProcessingRenderer _postProcessingRenderer;

    // Sync
    vkCore::Sync _sync;

    vkCore::Swapchain _swapchain;
    vkCore::CommandBuffer _swapchainCommandBuffers;

    std::shared_ptr<Gui> _gui = nullptr;

    Scene _scene;
    Settings _settings;
  };
} // namespace RAYEX_NAMESPACE
