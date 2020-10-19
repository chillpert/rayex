#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  /// Exposes all graphic settings supported by the renderer.
  ///
  /// Any necessary pipeline recreations and swapchain recreations will not be performed at the point of calling any setter but instead the next time the renderer
  /// will be updated.
  /// @todo Add a setUseTotalPathsOnly( bool flag )
  class RX_API Settings
  {
  public:
    friend class Api;
    friend class Rayex;
    friend class Pipeline;
    friend class Scene;

    /// @return Returns the recursion depth.
    [[nodiscard]] auto getRecursionDepth( ) const -> uint32_t { return this->recursionDepth; }

    /// Used to set the recursion depth.
    ///
    /// The function will trigger a pipeline recreation as soon as possible unless it was explicitely disabled using setAutomaticPipelineRefresh(bool).
    /// If a value higher than the device's maximum supported value is set, it will use the maximum value instead.
    /// @param recursionDepth The new value for the recursion depth.
    void setRecursionDepth( uint32_t recursionDepth );

    /// @return Returns the clear color.
    [[nodiscard]] auto getClearColor( ) const -> const glm::vec4& { return this->clearColor; }

    /// Used to changed the clear color.
    ///
    /// The function will trigger a swapchain recreation as soon as possible unless it was explicitely disabled using setAutomaticPipelineRefresh(bool).
    /// @param clearColor The new value for the clear color.
    void setClearColor( const glm::vec4& clearColor );

    /// @return Returns the path to assets.
    [[nodiscard]] auto getAssetsPath( ) const -> std::string_view { return this->assetsPath; }

    /// Used to set a path to the directory containing all assets.
    ///
    /// This path should contain all models, textures and shaders.
    /// @param argc The argc parameter that can be retrieved from the main-function's parameters.
    /// @param argv The argv parameter that can be retrieved from the main-function's parameters.
    void setAssetsPath( int argc, char* argv[] );

    /// Used to set a path to the directory containing all assets.
    ///
    /// This path should contain all models, textures and shaders.
    /// @param path The path to assets.
    void setAssetsPath( std::string_view path );

    /// @return Returns true if ray tracing is enabled and false if rasterization is enabled.
    /// @todo This function will be pointless once the new pipeline system is implemented.
    [[nodiscard]] auto getRayTracingEnabled( ) const -> bool { return this->rayTrace; }

    /// Used to enable or disable ray tracing.
    ///
    /// The function will trigger a swapchain recreation as soon as possible unless it was explicitely disabled using setAutomaticPipelineRefresh(bool).
    /// @param flag If false, ray tracing will be disabled.
    /// @todo This function will be pointless once the new pipeline system is implemented.
    void setEnableRayTracing( bool flag );

    /// Used to toggle the automatic pipeline recreation.
    /// @param flag If false, the pipelines will not be recreated automatically until this function is called with true.
    void setAutomaticPipelineRefresh( bool flag );

    /// Used to set the maximum amount of directional lights that can be used.
    /// @param amount The amount of maximum directional lights.
    void setMaxDirectionalLights( uint32_t amount );

    /// Used to set the maximum amount of point lights that can be used.
    /// @param amount The amount of maximum point lights.
    void setMaxPointLights( uint32_t amount );

    /// Used to set the maximum amount of geometrys that can be used.
    /// @param amount The amount of maximum geometrys.
    void setMaxGeometryInstances( uint32_t amount );

    /// @return Returns the maximum recursion depth on the GPU.
    [[nodiscard]] auto getMaxRecursionDepth( ) const -> uint32_t { return this->maxRecursionDepth; }

    /// Used to toggle the jitter cam.
    ///
    /// A jitter cam can be used for anti aliasing a static scene.
    /// A ray that will be cast into the scene normally starts off at the exact same position.
    /// The jitter cam will jitter or scatter the ray's starting position slightly using random values.
    /// @param flag Enable or disable the jitter cam.
    /// @note Disables SSAA automatically.
    void setEnableJitterCam( bool flag );

    /// @return Returns true, if jitter cam is enabled.
    [[nodiscard]] auto getJitterCamEnabled( ) const -> bool { return this->jitterCamEnabled; }

    /// Used to set the jitter cam's sample rate.
    /// @param sampleRate The jitter cam's sample rate.
    /// @see setJitterCamSampleRatePerRayGen(uint32_t)
    void setJitterCamSampleRate( uint32_t sampleRate );

    /// @return Returns the jitter cam's sample rate .
    [[nodiscard]] auto getJitterCamSampleRate( ) const -> uint32_t { return this->jitterCamSampleRate; }

    /// Used to set the jitter cam's sample rate per raygen.
    /// @param sampleRate Used to make the jitter cam faster by casting n rays directly in the raygen shader.
    /// @note It is advised to set a higher value for sampleRatePerRayGen and lowering sampleRate instead.
    /// @see setJitterCamSamplesRate(uint32_t)
    void setJitterCamSampleRatePerRayGen( uint32_t sampleRate );

    /// @return Returns the jitter cam's sample rate per raygen.
    [[nodiscard]] auto getJitterCamSampleRatePerRayGen( ) const -> uint32_t { return this->jitterCamSampleRatePerRayGen; }

    /// Used to toggle SSAA.
    /// @param flag If true, SSAA will be activated.
    /// @note Disables jitter cam automatically.
    void setEnableSsaa( bool flag );

    /// @return Returns true, if SSAA is enabled.
    [[nodiscard]] auto getSsaaEnabled( ) const -> bool { return this->ssaaEnabled; }

    /// Used to set the SSAA sample rate.
    /// @param sampleRate The desired SSAA sample rate. If this value is zero, then the sample rate will be set to one.
    void setSsaaSampleRate( uint32_t sampleRate );

    /// @return Returns the SSAA sample rate.
    [[nodiscard]] auto getSsaaSampleRate( ) const -> uint32_t { return this->ssaaSampleRate; }

  private:
    /// This function will be called by Rayex::init() in case the path was not set manually.
    /// @warning This function might file in setting the correct path. That is why it is recommended to set it automatically using setAssetsPath(std::string).
    void setDefaultAssetsPath( );

    bool rayTrace         = true;  ///< If true renderer will use ray tracing, if false it will use rasterization.
    bool refreshPipeline  = false; ///< Keeps track of whether or not the graphics pipeline needs to be recreated.
    bool refreshSwapchain = false; ///< Keeps track of whether or not the swapchain needs to be recreated.

    std::optional<uint32_t> maxDirectionalLights; ///< Can be set to avoid pipeline recreation everytime a directional light is added.
    std::optional<uint32_t> maxPointLights;       ///< Can be set to avoid pipeline recreation everytime a point light is added.
    std::optional<uint32_t> maxGeometryInstances; ///< Can be set to avoid pipeline recreation everytime a geometry instance is added.

    std::string assetsPath; ///< Where all assets like models, textures and shaders are stored.

    glm::vec4 clearColor                  = glm::vec4( 0.45F, 0.45F, 0.45F, 1.0F ); ///< Stores the clear color.
    uint32_t maxRecursionDepth            = 2;                                      ///< The maximum recursion depth.
    uint32_t recursionDepth               = 2;                                      ///< The current recursion depth.
    bool jitterCamEnabled                 = false;                                  ///< Keeps track of whether or not the jitter cam is enabled.
    uint32_t jitterCamSampleRate          = 100;                                    ///< Stores the sample rate of the jitter cam.
    uint32_t jitterCamSampleRatePerRayGen = 1;                                      ///< Stores the sample rate per raygen invocation.
    bool ssaaEnabled                      = true;                                   ///< Keeps track of whether or not SSAA is enabled.
    uint32_t ssaaSampleRate               = 2;                                      ///< Stores the amount of samples per pixel for SSAA.

    bool automaticPipelineRefresh  = false; ///< Keeps track of whether or not the graphics pipelines should be recreated automatically as soon as possible.
    bool automaticSwapchainRefresh = false; ///< Keeps track of whether or not the swapchain should be recreated automatically as soon as possible.
  };
} // namespace RAYEX_NAMESPACE

#endif // SETTINGS_HPP
