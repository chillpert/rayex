#ifndef MODEL_BASE_HPP
#define MODEL_BASE_HPP

#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/StorageBuffer.hpp"
#include "api/buffers/UniformBuffer.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/image/Texture.hpp"
#include "api/misc/Vertex.hpp"
#include "base/Mesh.hpp"
#include "modules/Modules.hpp"

namespace RAYEXEC_NAMESPACE
{
  RX_API std::shared_ptr<Geometry> loadObj( std::string_view path );
} // namespace RAYEXEC_NAMESPACE

#endif // MODEL_BASE_HPP
