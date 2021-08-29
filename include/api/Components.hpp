#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "base/Geometry.hpp"
#include "base/Window.hpp"

namespace RAYEX_NAMESPACE
{
  namespace components
  {
    extern int frameCount; ///< Stores the current frame count required by the jitter cam.

    extern std::string assetsPath; ///< Path to where models and textures are stored.
    extern std::string shaderPath; ///< Path to where shaders are stored.
    extern uint32_t geometryIndex;
    extern uint32_t materialIndex;
    extern uint32_t textureIndex;
    const size_t maxResources = 2;
    extern std::vector<Material> _materials; //? @todo this should not be global

  } // namespace components
} // namespace RAYEX_NAMESPACE

#endif // COMPONENTS_HPP
