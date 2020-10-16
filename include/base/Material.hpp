#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Contains all rendering properties and textures.
  struct Material
  {
    glm::vec4 ambient  = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 ambient  + vec1 texture index
    glm::vec4 diffuse  = glm::vec4( 0.2F, 1.0F, 1.0F, -1.0F ); // vec3 diffuse  + vec1 texture index
    glm::vec4 specular = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 specular + vec1 texture index
  };
} // namespace RAYEXEC_NAMESPACE

#endif // MAterial_HPP
