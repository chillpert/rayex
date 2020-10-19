#ifndef SCENE_HPP
#define SCENE_HPP

#include "base/Geometry.hpp"
#include "base/Lights.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct Scene
  {
    std::vector<std::shared_ptr<Geometry>> geometries;
    std::vector<std::shared_ptr<GeometryInstance>> geometryInstances;
    std::vector<std::shared_ptr<DirectionalLight>> directionalLights;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SCENE_HPP
