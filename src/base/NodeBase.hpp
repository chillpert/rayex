#ifndef NODE_HPP
#define NODE_HPP

#include "Model.hpp"

namespace RX
{
  class TransformNodeBase
  {
  public:
    TransformNodeBase() :
      m_worldTransform(glm::mat4(1.0f)),
      m_localTransform(glm::mat4(1.0f)),
      m_inverseWorldTransform(glm::mat4(1.0f)),
      m_inverseLocalTransform(glm::mat4(1.0f)) { }

    virtual ~TransformNodeBase() = default;

    glm::mat4 m_worldTransform;
    glm::mat4 m_localTransform;

    glm::mat4 m_inverseWorldTransform;
    glm::mat4 m_inverseLocalTransform;
  };

  class GeometryNodeBase : public TransformNodeBase
  {
  public:
    GeometryNodeBase() = default;
    GeometryNodeBase(const std::string& modelPath, const Material& material) :
      m_modelPath(modelPath), m_material(material) { }

    virtual ~GeometryNodeBase() = default;

    std::string m_modelPath;
    Material m_material;

    UniformBuffer m_uniformBuffers;
    
    // Ray tracing geometry instance data.
    uint32_t m_instanceId : 24;
    uint32_t m_mask : 8;
    uint32_t m_instanceOffset : 24;
    uint32_t m_flags : 8;
    uint64_t m_asHandle;
  };
}

#endif // NODE_HPP