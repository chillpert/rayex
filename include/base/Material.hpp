#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Contains all rendering properties and textures.
  struct Material
  {
    glm::vec3 ambient       = { };
    glm::vec3 diffuse       = { };
    glm::vec3 specular      = { };
    glm::vec3 transmittance = { };
    glm::vec3 emission      = { };

    float shininess       = 1.0f;
    float reflectionIndex = 1.0f;
    float opacity         = 1.0f;

    std::vector<uint32_t> diffuseTextures;
    std::vector<uint32_t> specularTextures;
    std::vector<uint32_t> bumpTextures;
    std::vector<uint32_t> displacementTextures;
    std::vector<uint32_t> alphaTextures;
    std::vector<uint32_t> reflectionTextures;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // MAterial_HPP
