#ifndef GEOMETRY_NODE_BASE_HPP
#define GEOMETRY_NODE_BASE_HPP

#include "Node.hpp"
#include "api/Pipeline.hpp"
#include "base/Material.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Contains all instance related ray tracing information.
  /// @ingroup API
  /*
  struct RayTracingInstance
  {
    glm::mat4 transform   = glm::mat4( 1.0F ); ///< World transform matrix
    glm::mat4 transformIT = glm::mat4( 1.0F ); ///< Inversed transposed world transform amtrix
    uint32_t modelIndex   = 0;                 ///< Reference to RAYEXEC_NAMESPACE::Api::models

    // Also works as padding.
    uint32_t baseNodeId     = 0; ///< A unique id assigned to any node.
    uint32_t geometryNodeId = 0; ///< A unique id assigned only to geometry nodes.
  };
  */

  /// Adds geometry to the TransformNode parent class.
  /// @ingroup Base
  class GeometryNode : public TransformNode
  {
  };
} // namespace RAYEXEC_NAMESPACE

#endif // GEOMETRY_NODE_BASE_HPP
