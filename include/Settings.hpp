#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Exposes all graphic settings supported by the renderer.
  ///
  /// If the client makes changes that require a pipeline or swapchain recreation, refresh must be set to true.
  /// @warning The client has to call either setAssetsPath(argc, argv) or setAssetsPath(path) for the renderer to work.
  /// @todo Split refresh into refreshPipeline and refreshSwapchain to avoid doing unnecessary operations.
  /// @todo Add some setting, that I don't remember anymore.
  class RX_API Settings
  {
  public:
    Settings( );

    friend class Api;
    friend class Pipeline;

    /// @return Returns the current recursion depth.
    [[nodiscard]] inline auto getRecursionDepth( ) const -> uint32_t { return this->recursionDepth; }

    /// Used to set the current recursion depth.
    /// @param maxRecursionDepth The new value for the maxium recursion depth.
    void setRecursionDepth( uint32_t recursionDepth );

    /// @return Returns the clear color.
    [[nodiscard]] inline auto getClearColor( ) const -> const glm::vec4& { return this->clearColor; }

    /// Used to changed the clear color.
    /// @param clearColor The new value for the clear color.
    void setClearColor( const glm::vec4& clearColor );

    /// @return Returns the path to assets.
    [[nodiscard]] inline auto getAssetsPath( ) const -> std::string_view { return this->assetsPath; }

    /// Used to set a path to assets.
    /// @param argc The argc parameter that can be retrieved from the main-function's parameters.
    /// @param argv The argv parameter that can be retrieved from the main-function's parameters.
    void setAssetsPath( int argc, char* argv[] );

    /// Used to set a path to assets.
    /// @param path The path to assets.
    void setAssetsPath( std::string_view path );

    /// @return Returns true if ray tracing is enabled and false if rasterization is enabled.
    [[nodiscard]] auto getRayTracingEnabled( ) const -> bool { return this->rayTrace; }

    /// Used to enable or disable ray tracing.
    void setEnableRayTracing( bool flag );

    /// Used to toggle the automatic pipeline recreation.
    /// @flag If false, the pipelines will not be recreated until this function is called with true.
    void setAutomaticPipelineRefresh( bool flag );

    /// Used to set a certain amount of directional light nodes that can be used.
    ///
    /// It is not necessary to use this function as the shaders will automatically will be updated to support any given
    /// amount of light nodes. However, this process requires pipeline recreation and therefore it is recommended to set
    /// the anticipated amount of directional light nodes in the scene to avoid pipeline recreation.
    /// @param amount The amount of anticipated directional light nodes.
    void setMaxDirectionalLights( uint32_t amount );

    /// Used to set a certain amount of point light nodes that can be used.
    ///
    /// It is not necessary to use this function as the shaders will automatically will be updated to support any given
    /// amount of light nodes. However, this process requires pipeline recreation and therefore it is recommended to set
    /// the anticipated amount of point light nodes in the scene to avoid pipeline recreation.
    /// @param amount The amount of anticipated point light nodes.
    void setMaxPointLights( uint32_t amount );

    void setMaxGeometryNodes( uint32_t amount );

    /// @return Returns the maximum recursion depth on the GPU.
    [[nodiscard]] auto getMaxRecursionDepth( ) const -> uint32_t { return this->maxRecursionDepth; }

  private:
    bool rayTrace         = true;  ///< If true renderer will use ray tracing, if false it will use rasterization.
    bool refreshPipeline  = false; ///< Keeps track of whether or not the graphics pipeline needs to be recreated.
    bool refreshSwapchain = false; ///< Keeps track of whether or not the swapchain needs to be recreated.

    std::optional<uint32_t> maxDirectionalLights; ///< Can be set to avoid pipeline recreation everytime a directional light is added.
    std::optional<uint32_t> maxPointLights;       ///< Can be set to avoid pipeline recreation everytime a point light is added.
    std::optional<uint32_t> maxGeometryNodes;     ///< Can be set to avoid pipeline recreation everytime a geometry node is added.

    glm::vec4 clearColor       = glm::vec4( 0.45F, 0.45F, 0.45F, 1.0F ); ///< Stores the clear color.
    uint32_t maxRecursionDepth = 4;                                      ///< The maximum recursion depth.
    uint32_t recursionDepth    = 4;                                      ///< The current recursion depth.
    std::string assetsPath;                                              ///< Where all assets like models, textures and shaders are stored.

    bool automaticPipelineRefresh  = false;
    bool automaticSwapchainRefresh = false;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SETTINGS_HPP