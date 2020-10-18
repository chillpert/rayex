#ifndef SCENE_HPP
#define SCENE_HPP

#include "base/Interfaces.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct Scene
  {
    std::vector<std::shared_ptr<Geometry>> geometries;
    std::vector<std::shared_ptr<GeometryInstance>> geometryInstances;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SCENE_HPP
