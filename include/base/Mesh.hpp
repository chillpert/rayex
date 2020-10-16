#ifndef MESH_HPP
#define MESH_HPP

#include "base/Material.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct Mesh
  {
    size_t vertexOffset;
    Material material;
  };
} // namespace RAYEXEC_NAMESPACE

#endif // MESH_HPP
