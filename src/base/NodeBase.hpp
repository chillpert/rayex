#ifndef NODEBASE_HPP
#define NODEBASE_HPP

#include "stdafx.hpp"

namespace rx
{
  static size_t nodeCounter = 0; ///< For assigning each node a unique ID.

  /// Describes a node in the scene graph.
  /// @ingroup Base
  class Node
  {
  public:
    Node( ) :
      m_id( ++nodeCounter ) { }
    virtual ~Node( ) = default;

    /// @return The node's ID.
    size_t getID( ) { return m_id; }

  private:
    size_t m_id; ///< The node's unique ID.
  };

  /// Adds transformation abilities to the Node base class.
  /// @ingroup Base
  class TransformNode : public Node
  {
  public:
    virtual ~TransformNode( ) = default;

    glm::mat4 m_worldTransform = glm::mat4( 1.0f ); ///< The world space transformation matrix.
    glm::mat4 m_localTransform = glm::mat4( 1.0f ); ///< The local space transformation matrix.

    glm::mat4 m_inverseWorldTransform = glm::mat4( 1.0f ); ///< The world space transformation matrix inversed.
    glm::mat4 m_inverseLocalTransform = glm::mat4( 1.0f ); ///< The local space transformation matrix inversed.
  };
}

#endif // NODEBASE_HPP