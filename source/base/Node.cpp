#include "base/Node.hpp"

namespace RAYEXEC_NAMESPACE
{
  std::unordered_set<std::string> Node::names;

  Node::Node( std::string_view name ) :
    id( ++nodeCounter )
  {
    auto result = names.insert( std::string( name ) );
    RX_ASSERT( result.second, "Node must have a unique name." );
  }

  void Node::setParent( std::shared_ptr<Node> newParent )
  {
    auto thisNode = shared_from_this( );

    // 1. Remove this node from its old parent's children list.
    auto oldParent = this->parent;
    if ( oldParent != nullptr )
    {
      for ( auto iter = oldParent->children.begin( ); iter != oldParent->children.end( ); ++iter )
      {
        if ( ( *iter ) == thisNode )
        {
          oldParent->children.erase( iter );
          break;
        }
      }
    }

    // 2. Add this node to it's new parent's children list.
    newParent->addChild( thisNode );

    // 3. Set this node's parent to new parent.
    this->parent = newParent;
  }

  void Node::addChild( std::shared_ptr<Node> newChild )
  {
    // 1. Delete new child from it's original parent node's children's list.
    auto oldParent = newChild->parent;
    if ( oldParent != nullptr )
    {
      for ( auto iter = newChild->parent->children.begin( ); iter != newChild->parent->children.end( ); ++iter )
      {
        if ( ( *iter ) == newChild )
        {
          newChild->parent->children.erase( iter );
          break;
        }
      }
    }

    // 2. Set the child's parent to this node.
    newChild->setParent( shared_from_this( ) );

    // 3. Add the new child to this node's children.
    this->children.push_back( newChild );
  }

  TransformNode::TransformNode( std::string_view name ) :
    Node( name ) {}
} // namespace RAYEXEC_NAMESPACE
