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
    std::shared_ptr<Node> root;

    // Models
    std::vector<Texture> textures;
    std::vector<Material> materials;

    //std::vector<std::string> modelPaths; ///< A vector containing all models' paths.

    std::vector<std::shared_ptr<Geometry>> geometries;
    std::vector<GeometryInstance> geometryInstances;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SCENE_HPP
