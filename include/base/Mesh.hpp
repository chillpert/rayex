#ifndef MESH_HPP
#define MESH_HPP

#include "base/Material.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct Mesh
  {
    Material material = { };

    glm::vec4 padding0 = glm::vec4( 1.0F );
    glm::vec4 padding1 = glm::vec4( 1.0F );
    glm::vec4 padding2 = glm::vec4( 1.0F );
    uint32_t padding3  = 0;
    uint32_t padding4  = 0;
    uint32_t padding5  = 0;

    uint32_t indexOffset = 0;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // MESH_HPP
