#include "base/GeometryNode.hpp"

namespace RAYEXEC_NAMESPACE
{
  static uint32_t geometryNodeCounter = 0;

  GeometryNode::GeometryNode( std::string_view modelPath, const Material& material ) :
    modelPath( modelPath ),
    material( material )
  {
    this->rtInstance.geometryNodeId = geometryNodeCounter;
    ++geometryNodeCounter;
  }
} // namespace RAYEXEC_NAMESPACE