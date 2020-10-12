#include "base/GeometryNode.hpp"

namespace RAYEXEC_NAMESPACE
{
  static uint32_t geometryNodeCounter = 0;

  GeometryNode::GeometryNode( std::string_view name, std::string_view modelPath ) :
    TransformNode( name ),
    modelPath( modelPath )
  {
    this->rtInstance.geometryNodeId = geometryNodeCounter;
    ++geometryNodeCounter;
  }
} // namespace RAYEXEC_NAMESPACE
