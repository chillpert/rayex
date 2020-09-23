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
    glm::mat4 transform = glm::mat4( 1.0f );
    glm::mat4 transformIT = glm::mat4( 1.0f );
    uint32_t modelIndex = 0;  ///< Reference to RAYEXEC_NAMESPACE::Api::models
    uint32_t textureOffset = 0; ///< Offset in RAYEXEC_NAMESPACE::Api::textures
    float padding0;
    float padding1;
  };

  /// Adds geometry to the TransformNode parent class.
  /// @ingroup Base
  class GeometryNode : public TransformNode
  {
  public:
    /// @param modelPath @see modelPath
    /// @param material @see material
    GeometryNode( const std::string& modelPath, const Material& material ) :
      modelPath( modelPath ), material( material )
    { }

    virtual ~GeometryNode( ) = default;

    std::string modelPath; ///< The relative path to the model file.
    Material material; ///< The material defining rendering properties.

    RayTracingInstance rtInstance; ///< @see RayTracingInstance

    UniformBuffer rsUniformBuffer;
  };
}

#endif // GEOMETRY_NODE_BASE_HPP