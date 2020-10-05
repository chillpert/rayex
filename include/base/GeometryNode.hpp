#ifndef GEOMETRY_NODE_BASE_HPP
#define GEOMETRY_NODE_BASE_HPP

#include "Node.hpp"
#include "base/Material.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Contains all instance related ray tracing information.
  /// @ingroup API
  struct RayTracingInstance
  {
    glm::mat4 transform    = glm::mat4( 1.0F ); ///< World transform matrix
    glm::mat4 transformIT  = glm::mat4( 1.0F ); ///< Inversed transposed world transform amtrix
    uint32_t modelIndex    = 0;                 ///< Reference to RAYEXEC_NAMESPACE::Api::models
    uint32_t textureOffset = 0;                 ///< Offset in RAYEXEC_NAMESPACE::Api::textures

    // Also works as padding.
    uint32_t baseNodeId     = 0;
    uint32_t geometryNodeId = 0;
  };

  /// Adds geometry to the TransformNode parent class.
  /// @ingroup Base
  class GeometryNode : public TransformNode
  {
  public:
    /// @param modelPath @see modelPath
    /// @param material @see material
    RX_API GeometryNode( std::string_view modelPath, const Material& material );

    ~GeometryNode( ) override = default;

    GeometryNode( const GeometryNode& )  = delete;
    GeometryNode( const GeometryNode&& ) = delete;

    auto operator=( const GeometryNode& ) -> GeometryNode& = delete;
    auto operator=( const GeometryNode && ) -> GeometryNode& = delete;

    [[nodiscard]] auto getType( ) const -> NodeType override { return NodeType::eGeometryNode; }

    std::string modelPath; ///< The relative path to the model file.
    Material material;     ///< The material defining rendering properties.

    RayTracingInstance rtInstance; ///< @see RayTracingInstance
  };
} // namespace RAYEXEC_NAMESPACE

#endif // GEOMETRY_NODE_BASE_HPP