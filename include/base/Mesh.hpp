#ifndef MESH_HPP
#define MESH_HPP

#include "base/Material.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct Mesh
  {
    Material material;
    size_t indexOffset;

    float padding0 = 0.0F;
    float padding1 = 0.0F;
    float padding2 = 0.0F;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // MESH_HPP
