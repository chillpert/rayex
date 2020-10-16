#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Contains all rendering properties and textures.
  struct Material
  {
    glm::vec4 ambient  = { }; // vec3 ambient  + vec1 texture index
    glm::vec4 diffuse  = { }; // vec3 diffuse  + vec1 texture index
    glm::vec4 specular = { }; // vec3 specular + vec1 texture index
  };
} // namespace RAYEXEC_NAMESPACE

#endif // MAterial_HPP
