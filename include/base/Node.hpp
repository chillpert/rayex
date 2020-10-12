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
  class Node : public std::enable_shared_from_this<Node>
  {
  public:
    Node( std::string_view name );
    virtual ~Node( ) = default;

    Node( const Node& )  = delete;
    Node( const Node&& ) = delete;

    auto operator=( const Node& ) -> Node& = delete;
    auto operator=( const Node && ) -> Node& = delete;

    [[nodiscard]] virtual auto getType( ) const -> NodeType { return NodeType::eNode; }

    /// @return The node's ID.
    [[nodiscard]] auto getID( ) -> uint32_t { return id; }

    void setParent( std::shared_ptr<Node> newParent );
    void addChild( std::shared_ptr<Node> newChild );

  private:
    uint32_t id;

    std::list<std::shared_ptr<Node>> children;
    std::shared_ptr<Node> parent;

    static std::unordered_set<std::string> names;
  };

  /// Adds transformation abilities to the Node base class.
  /// @ingroup Base
  class TransformNode : public Node
  {
  public:
    TransformNode( std::string_view name );
    ~TransformNode( ) override = default;

    TransformNode( const TransformNode& )  = delete;
    TransformNode( const TransformNode&& ) = delete;

    auto operator=( const TransformNode& ) -> TransformNode& = delete;
    auto operator=( const TransformNode && ) -> TransformNode& = delete;

    [[nodiscard]] auto getType( ) const -> NodeType override { return NodeType::eTransformNode; }

    glm::mat4 worldTransform = glm::mat4( 1.0F ); ///< The world space transformation matrix.
    glm::mat4 localTransform = glm::mat4( 1.0F ); ///< The local space transformation matrix.

    glm::mat4 inverseWorldTransform = glm::mat4( 1.0F ); ///< The world space transformation matrix inversed.
    glm::mat4 inverseLocalTransform = glm::mat4( 1.0F ); ///< The local space transformation matrix inversed.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // Node_HPP
