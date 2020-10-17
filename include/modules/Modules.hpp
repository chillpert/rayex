#ifndef MODULES_HPP
#define MODULES_HPP

#include "api/misc/Vertex.hpp"
#include "base/Mesh.hpp"
#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct Geometry
  {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Mesh> meshes;

    uint32_t geometryIndex = 0;
    std::string_view path;
  };

  struct GeometryInstance
  {
    glm::mat4 transform    = glm::mat4( 1.0F );
    glm::mat4 transformIT  = glm::mat4( 1.0F );
    uint32_t geometryIndex = 0;

  private:
    float padding0 = 0.0F;
    float padding1 = 0.0F;
    float padding2 = 0.0F;
  };

  struct Light
  {
    glm::vec3 ambient  = glm::vec3( 1.0F );
    glm::vec3 diffuse  = glm::vec3( 1.0F );
    glm::vec3 specular = glm::vec3( 1.0F );

    float ambientIntensity  = 0.0F;
    float diffuseIntensity  = 0.0F;
    float specularIntensity = 0.0F;
  };

  struct DirectionalLight : public Light
  {
    glm::vec3 direction = glm::vec3( 1.0F );
  };

  struct PointLight : public Light
  {
    glm::vec3 position = glm::vec3( 1.0F );

    float constant  = 0.0F;
    float linear    = 0.0F;
    float quadratic = 0.0F;
  };

} // namespace RAYEXEC_NAMESPACE

#endif // MODULES_HPP
