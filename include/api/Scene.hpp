#ifndef SCENE_HPP
#define SCENE_HPP

#include "api/image/Texture.hpp"
#include "base/GeometryNode.hpp"
#include "base/LightNode.hpp"
#include "base/Model.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct Scene
  {
    std::vector<std::shared_ptr<Geometry>> geometries;
    std::vector<std::shared_ptr<GeometryInstance>> geometryInstances;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SCENE_HPP
