#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Exposes all graphic settings supported by the renderer.
  ///
  /// If the client makes changes that require a pipeline or swapchain recreation, refresh must be set to true.
  /// @warning The client has to call either setResourcePath(argc, argv) or setResourcePath(path) for the renderer to work.
  /// @todo Split refresh into refreshPipeline and refreshSwapchain to avoid doing unnecessary operations.
  /// @todo Add some setting, that I don't remember anymore. 
  class RX_API Settings
  {
  public:
    Settings( );

    /// @return Returns the maximum recursion depth.
    inline uint32_t getMaxRecursionDepth( ) const { return maxRecursionDepth; }

    /// Used to set the maximum recursion depth.
    /// @param maxRecursionDepth The new value for the maxium recursion depth.
    /// @param refresh If true, the pipeline and or swapchain will be re-created without having to set refresh to true manually.
    void setMaxRecursionDepth( uint32_t maxRecursionDepth, bool refresh = true );

    /// @return Returns the clear color.
    inline const glm::vec4& getClearColor( ) const { return clearColor; }

    /// Used to changed the clear color.
    /// @param clearColor The new value for the clear color.
    /// @param refresh If true, the pipeline and or swapchain will be re-created without having to set refresh to true manually.
    void setClearColor( const glm::vec4& clearColor, bool refresh = true );

    /// @return Returns the path to resources.
    inline const std::string& getResourcePath( ) const { return resourcePath; }
    
    /// Used to set a path to resources.
    /// @param argc The argc parameter that can be retrieved from the main-function's parameters.
    /// @param argv The argv parameter that can be retrieved from the main-function's parameters.
    void setResourcePath( int argc, char* argv[] ); 

    /// Used to set a path to resources.
    /// @param path The path to resources.
    void setResourcePath( const std::string& path );

    bool refresh = false; ///< Keeps track of whether or not the graphics pipeline needs to be re-created.
    bool rayTrace = true; ///< If true renderer will use ray tracing, if false it will use rasterization.

  private:
    glm::vec4 clearColor = glm::vec4( 0.45f, 0.45f, 0.45f, 1.0f ); ///< Stores the clear color.
    uint32_t maxRecursionDepth = 4; ///< The maximum recursion depth.
    std::string resourcePath; ///< Where all resources like models, textures and shaders are stored.

  };
}

#endif // SETTINGS_HPP