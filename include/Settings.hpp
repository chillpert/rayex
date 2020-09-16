#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// Exposes all graphic settings supported by the renderer.
  ///
  /// If the client makes changes that require a pipeline or swapchain recreation, s_refresh must be set to true.
  /// @warning The client has to call either setResourcePath(argc, argv) or setResourcePath(path) for the renderer to work.
  /// @todo Split s_refresh into s_refreshPipeline and s_refreshSwapchain to avoid doing unnecessary operations.
  /// @todo Add some setting, that I don't remember anymore. 
  class RX_API Settings
  {
  public:
    /// @return Returns the path to resources.
    static inline const std::string& getResourcePath( ) { return s_resourcePath; }
    
    /// Used to set a path to resources.
    /// @param argc The argc parameter that can be retrieved from the main-function's parameters.
    /// @param argv The argv parameter that can be retrieved from the main-function's parameters.
    static void setResourcePath( int argc, char* argv[] ); 

    /// Used to set a path to resources.
    /// @param path The path to resources.
    static void setResourcePath( const std::string& path );

    static bool s_refresh; ///< Keeps track of whether or not the graphics pipeline needs to be re-created.
    static uint32_t s_maxRecursionDepth; ///< The maximum recursion depth.
    static glm::vec4 s_clearColor; ///< Stores the clear color.
  
  private:
    static std::string s_resourcePath; ///< Where all resources like models, textures and shaders are stored.
  };
}

#endif // SETTINGS_HPP