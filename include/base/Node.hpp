#ifndef Node_HPP
#define Node_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  static size_t nodeCounter = 0; ///< For assigning each node a unique ID.

  /// Describes a node in the scene graph.
  /// @ingroup Base
  class Node
  {
  public:
    Node( ) :
      id( ++nodeCounter ) {}
    virtual ~Node( ) = default;

    /// @return The node's ID.
    size_t getID( ) { return id; }

  private:
    size_t id; ///< The node's unique ID.
  };

  /// Adds transformation abilities to the Node base class.
  /// @ingroup Base
  class TransformNode : public Node
  {
  public:
    virtual ~TransformNode( ) = default;

    glm::mat4 worldTransform = glm::mat4( 1.0f ); ///< The world space transformation matrix.
    glm::mat4 localTransform = glm::mat4( 1.0f ); ///< The local space transformation matrix.

    glm::mat4 inverseWorldTransform = glm::mat4( 1.0f ); ///< The world space transformation matrix inversed.
    glm::mat4 inverseLocalTransform = glm::mat4( 1.0f ); ///< The local space transformation matrix inversed.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // Node_HPP