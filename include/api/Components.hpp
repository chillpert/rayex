#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "base/Window.hpp"

namespace RAYEX_NAMESPACE
{
  namespace components
  {
    extern int frameCount; ///< Stores the current frame count required by the jitter cam.

    extern std::string assetsPath; ///< Path to where shaders, models and textures are stored.
    extern uint32_t geometryIndex;
    extern uint32_t textureIndex;
    const size_t maxResources = 2;

  } // namespace components
} // namespace RAYEX_NAMESPACE

#endif // COMPONENTS_HPP
