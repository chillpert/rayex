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

    // Nodes to render
    std::list<std::shared_ptr<GeometryNode>> geometryNodes;         ///< A list of all geometry nodes to render.
    std::list<std::shared_ptr<DirectionalLightNode>> dirLightNodes; ///< A list of all directional light nodes to render.
    std::list<std::shared_ptr<PointLightNode>> pointLightNodes;     ///< A list of all point light nodes to render.

    // Models
    std::vector<std::shared_ptr<Model>> models; ///< A list of all models that can be rendered.
    std::vector<Texture> textures;
    std::vector<Material> materials;

    //std::vector<std::string> modelPaths; ///< A vector containing all models' paths.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // SCENE_HPP
