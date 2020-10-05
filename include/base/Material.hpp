#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Contains all rendering properties and textures.
  struct Material
  {
    Material( ) = default;
    RX_API Material( std::string_view diffuseTexture );

    glm::vec3 ambient       = { };
    glm::vec3 diffuse       = { };
    glm::vec3 specular      = { };
    glm::vec3 transmittance = { };
    glm::vec3 emission      = { };

    float shininess       = 1.0f;
    float reflectionIndex = 1.0f;
    float opacity         = 1.0f;

    std::vector<std::string> diffuseTextures;
    std::vector<std::string> specularTextures;
    std::vector<std::string> bumpTextures;
    std::vector<std::string> displacementTextures;
    std::vector<std::string> alphaTextures;
    std::vector<std::string> reflectionTextures;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // MAterial_HPP