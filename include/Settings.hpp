#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// Exposes all graphic settings supported by the renderer.
  ///
  /// If the client makes changes that require a pipeline or swapchain recreation, s_refresh must be set to true.
  /// @todo Split s_refresh into s_refreshPipeline and s_refreshSwapchain to avoid doing unnecessary operations.
  struct RX_API Settings
  {
    static bool s_refresh; ///< Keeps track of whether or not the graphics pipeline needs to be re-created.
    static uint32_t s_maxRecursionDepth; ///< The maximum recursion depth.
    static glm::vec4 s_clearColor; ///< Stores the clear color.
  };
}

#endif // SETTINGS_HPP