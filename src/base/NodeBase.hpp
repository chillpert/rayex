#ifndef NODEBASE_HPP
#define NODEBASE_HPP

#include "Model.hpp"

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

  /// Adds lighting-related information to the TransformNode parent class.
  /// @note This node only provides common data for its inherited classes and cannot be instantiated.
  /// @ingroup Base
  class LightNode : public TransformNode
  {
  public:
    virtual ~LightNode( ) = default;

    glm::vec3 m_ambient = { 0.2f, 0.2f, 0.2f }; ///< The ambient color.
    glm::vec3 m_diffuse = { 0.8f, 0.8f, 0.8f }; ///< The diffuse color.
    glm::vec3 m_specular = { 1.0f, 1.0f, 1.0f }; ///< The specular highlight's color.

    float m_ambientStrength = 0.7f; ///< The ambient intensity.
  };

  /// Can be used to add directional light sources to the scene.
  /// @ingroup Base
  class DirectionalLightNode : public LightNode
  {
  public:
    virtual ~DirectionalLightNode( ) = default;

    void addToUbos( DirectionalLightNodeUbos& ubos )
    {
      ubos.m_ambient.push_back( m_ambient );
      ubos.m_diffuse.push_back( m_diffuse );
      ubos.m_specular.push_back( m_specular );
      ubos.m_ambientStrength.push_back( m_ambientStrength );

      ubos.m_direction.push_back( m_direction );
    }

    glm::vec3 m_direction = { 1.0f, -1.0f, 1.0f }; ///< The direction the light is pointing at.
  };

  /// Can be used to add point light sources to the scene.
  /// @ingroup Base
  class PointLightNode : public LightNode
  {
  public:
    virtual ~PointLightNode( ) = default;

    void addToUbos( PointLightNodeUbos& ubos )
    {
      ubos.m_ambient.push_back( m_ambient );
      ubos.m_diffuse.push_back( m_diffuse );
      ubos.m_specular.push_back( m_specular );
      ubos.m_ambientStrength.push_back( m_ambientStrength );

      ubos.m_position.push_back( m_position );
      ubos.m_constant.push_back( m_constant );
      ubos.m_linear.push_back( m_linear );
      ubos.m_quadratic.push_back( m_quadratic );
    }


    glm::vec3 m_position = glm::vec3( 0.0f ); ///< The position of the light.

    float m_constant = 1.0f; ///< The constant factor of the light's abbreviation.
    float m_linear = 0.09f; ///< The linear factor of the light's abbreviation.
    float m_quadratic = 0.032f; ///< The quadratic factor of the light's abbreviation.
  };
}

#endif // NODEBASE_HPP