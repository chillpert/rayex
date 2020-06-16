#ifndef NODE_BASE_HPP
#define NODE_BASE_HPP

#include "ModelBase.hpp"

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
    GeometryNodeBase(const std::shared_ptr<ModelBase> model) :
      m_model(model) { }
    GeometryNodeBase(const std::shared_ptr<ModelBase> model, const Material& material) :
      m_model(model),
      m_material(material) { }

    virtual ~GeometryNodeBase() = default;

    std::shared_ptr<ModelBase> m_model = nullptr;
    Material m_material;
  };
}

#endif // NODE_BASE_HPP