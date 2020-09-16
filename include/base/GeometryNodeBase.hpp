#ifndef GEOMETRY_NODE_BASE_HPP
#define GEOMETRY_NODE_BASE_HPP

#include "NodeBase.hpp"
#include "base/Material.hpp"

namespace RENDERER_NAMESPACE
{
  /// Contains all instance related ray tracing information.
  struct RayTracingInstance
  {
    uint32_t m_modelIndex = 0;  ///< Reference to rx::Api::m_models
    uint32_t m_textureOffset = 0; ///< Offset in rx::Api::m_textures
  };

  /// Adds geometry to the TransformNode parent class.
  /// @ingroup Base
  class GeometryNode : public TransformNode
  {
  public:
    /// @param modelPath @see m_modelPath
    /// @param material @see m_material
    GeometryNode( const std::string& modelPath, const Material& material ) :
      m_modelPath( modelPath ), m_material( material )
    { }

    virtual ~GeometryNode( ) = default;

    std::string m_modelPath; ///< The relative path to the model file.
    Material m_material; ///< The material defining rendering properties.

    RayTracingInstance m_rtInstance; ///< @see RayTracingInstance
  };
}

#endif // GEOMETRY_NODE_BASE_HPP