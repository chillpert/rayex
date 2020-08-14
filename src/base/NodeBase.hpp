#ifndef NODEBASE_HPP
#define NODEBASE_HPP

#include "Model.hpp"

namespace rx
{
  // Easiest way of assigning each node a unique id.
  static size_t nodeCounter = 0;

  class Node
  {
  public:
    Node( ) :
      m_id( ++nodeCounter ) { }
    virtual ~Node( ) = default;

    size_t m_id;
  };

  class TransformNode : public Node
  {
  public:
    TransformNode( ) :
      m_worldTransform( glm::mat4( 1.0f ) ),
      m_localTransform( glm::mat4( 1.0f ) ),
      m_inverseWorldTransform( glm::mat4( 1.0f ) ),
      m_inverseLocalTransform( glm::mat4( 1.0f ) )
    { }

    virtual ~TransformNode( ) = default;

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

  class GeometryNode : public TransformNode
  {
  public:
    GeometryNode( ) = default;
    GeometryNode( const std::string& modelPath, const Material& material ) :
      m_modelPath( modelPath ), m_material( material )
    { }

    virtual ~GeometryNode( ) = default;

    std::string m_modelPath;
    Material m_material;

    UniformBuffer m_uniformBuffers;

    // Ray tracing geometry instance data.
    RaytracingInstance m_rtInstance;
  };

  class LightNode : public Node
  {
  public:
    virtual ~LightNode( ) = default;

    glm::vec3 m_ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 m_diffuse = { 0.8f, 0.8f, 0.8f };
    glm::vec3 m_specular = { 1.0f, 1.0f, 1.0f };

    float m_ambientStrength = 0.7f;
  };

  class DirectionalLightNode : public LightNode
  {
  private:
    struct PushConstant
    {
      glm::vec3 direction;
      float ambientStrength;
    };

  public:
    virtual ~DirectionalLightNode( ) = default;

    PushConstant toPushConstant( ) { return { m_direction, m_ambientStrength }; }

    glm::vec3 m_direction = { 1.0f, -1.0f, 1.0f };
  };
}

#endif // NODEBASE_HPP