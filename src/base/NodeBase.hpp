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

  struct RaytracingInstance
  {
    uint32_t m_instanceId;
    uint32_t m_mask;
    uint32_t m_instanceOffset;
    uint32_t m_flags;
    uint64_t m_asHandle;
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
    RaytracingInstance m_rtInstance;
  };
}

#endif // NODE_HPP