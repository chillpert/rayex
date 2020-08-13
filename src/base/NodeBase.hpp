#ifndef NODE_HPP
#define NODE_HPP

#include "Model.hpp"

namespace rx
{
  // Easiest way of assigning each node a unique id.
  static size_t nodeCounter = 0;

  class NodeBase
  {
  public:
    NodeBase( ) :
      m_id( ++nodeCounter ) { }
    virtual ~NodeBase( ) = default;

    size_t m_id;
  };

  class TransformNodeBase : public NodeBase
  {
  public:
    TransformNodeBase( ) :
      m_worldTransform( glm::mat4( 1.0f ) ),
      m_localTransform( glm::mat4( 1.0f ) ),
      m_inverseWorldTransform( glm::mat4( 1.0f ) ),
      m_inverseLocalTransform( glm::mat4( 1.0f ) )
    { }

    virtual ~TransformNodeBase( ) = default;

    glm::mat4 m_worldTransform;
    glm::mat4 m_localTransform;

    glm::mat4 m_inverseWorldTransform;
    glm::mat4 m_inverseLocalTransform;
  };

  struct RaytracingInstance
  {
    uint32_t m_objIndex = 0;  // Reference to the `m_objModel`
    uint32_t m_txtOffset = 0; // Offset in `m_textures`
  };

  class GeometryNodeBase : public TransformNodeBase
  {
  public:
    GeometryNodeBase( ) = default;
    GeometryNodeBase( const std::string& modelPath, const Material& material ) :
      m_modelPath( modelPath ), m_material( material )
    { }

    virtual ~GeometryNodeBase( ) = default;

    std::string m_modelPath;
    Material m_material;

    UniformBuffer m_uniformBuffers;

    // Ray tracing geometry instance data.
    RaytracingInstance m_rtInstance;
  };

  class LightNodeBase : public NodeBase
  {
  public:
    virtual ~LightNodeBase( ) = default;

    glm::vec3 m_ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 m_diffuse = { 0.8f, 0.8f, 0.8f };
    glm::vec3 m_specular = { 1.0f, 1.0f, 1.0f };

    float m_ambientStrength = 0.7f;
  };

  class DirectionalLightNodeBase : public LightNodeBase
  {
  private:
    struct PushConstant
    {
      glm::vec3 direction;
      float ambientStrength;
    };

  public:
    virtual ~DirectionalLightNodeBase( ) = default;

    PushConstant toPushConstant( ) { return { m_direction, m_ambientStrength }; }

    glm::vec3 m_direction = { 1.0f, -1.0f, 1.0f };
  };
}

#endif // NODE_HPP