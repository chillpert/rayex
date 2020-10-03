#ifndef Node_HPP
#define Node_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  static uint32_t nodeCounter = 0; ///< For assigning each node a unique ID.

  enum class NodeType
  {
    eNode,
    eTransformNode,
    eGeometryNode,
    eLightNode,
    eDirectionalLightNode,
    ePointLightNode
  };

  /// Describes a node in the scene graph.
  /// @ingroup Base
  class Node
  {
  public:
    Node( ) :
      id( ++nodeCounter ) {}

    virtual ~Node( ) = default;
    [[nodiscard]] virtual auto getType( ) const -> NodeType { return NodeType::eNode; }

    /// @return The node's ID.
    [[nodiscard]] auto getID( ) -> uint32_t { return id; }

  private:
    uint32_t id; ///< The node's unique ID.
  };

  /// Adds transformation abilities to the Node base class.
  /// @ingroup Base
  class TransformNode : public Node
  {
  public:
    ~TransformNode( ) override = default;
    [[nodiscard]] auto getType( ) const -> NodeType override { return NodeType::eTransformNode; }

    glm::mat4 worldTransform = glm::mat4( 1.0F ); ///< The world space transformation matrix.
    glm::mat4 localTransform = glm::mat4( 1.0F ); ///< The local space transformation matrix.

    glm::mat4 inverseWorldTransform = glm::mat4( 1.0F ); ///< The world space transformation matrix inversed.
    glm::mat4 inverseLocalTransform = glm::mat4( 1.0F ); ///< The local space transformation matrix inversed.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // Node_HPP