#ifndef BASE_HPP
#define BASE_HPP

#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "DescriptorSet.hpp"
#include "Shader.hpp"
#include "Gui.hpp"
#include "Window.hpp"

/* 
  This file contains all programming interfaces required by the library. 
  All of the client's own implementations need be derived from these classes.

  The following APIs exist:
    - CameraBase
    - ModelBase
    - TransformNodeBase
    - GeometryNodeBase
    - GuiBase
    - WindowBase

  You can use the default implementations. 
  But for a custom one, simply inherit from any of these classes.
*/

namespace RX
{
  class CameraBase
  {
  public:
    CameraBase(const glm::ivec2& screenSize, const glm::vec3& position = glm::vec3(0.0f)) :
      m_screenSize(screenSize),
      m_position(position),
      m_view(glm::mat4(1.0f)),
      m_projection(glm::mat4(1.0f)) { }

    virtual ~CameraBase() = default;
    virtual void update() { };

    inline const glm::vec3& getPosition() { return m_position; }

    void setPosition(const glm::vec3& position)
    {
      m_position = position;
    }

    void setScreenSize(const glm::ivec2& screenSize)
    {
      m_screenSize = screenSize;
    }

    virtual inline const glm::mat4& getViewMatrix() { return m_view; }
    virtual inline const glm::mat4& getProjectionMatrix() { return m_projection; }

  protected:
    glm::ivec2 m_screenSize;
    glm::vec3 m_position;

    glm::mat4 m_view;
    glm::mat4 m_projection;
  };

  struct Material
  {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 transmittance;
    glm::vec3 emission;

    float shininess;
    float reflectionIndex;
    float opacity;

    std::string ambientTexture;
    std::string diffuseTexture;
    std::string specularTexture;
    std::string bumpTexture;
    std::string displacementTexture;
    std::string alphaTexture;
    std::string reflectionTexture;

    std::unordered_set<std::string> getTextures()
    {
      std::unordered_set<std::string> textures;
      
      if (!ambientTexture.empty())
        textures.insert(ambientTexture);

      if (!diffuseTexture.empty())
        textures.insert(diffuseTexture);

      if (!specularTexture.empty())
        textures.insert(specularTexture);

      if (!bumpTexture.empty())
        textures.insert(bumpTexture);

      if (!displacementTexture.empty())
        textures.insert(displacementTexture);

      if (!alphaTexture.empty())
        textures.insert(alphaTexture);

      if (!reflectionTexture.empty())
        textures.insert(reflectionTexture);

      return textures;
    }
  };

  class ModelBase
  {
  public:
    ModelBase(const std::string& path) :
      m_path(path)
    { 
      initialize();
    }

    virtual ~ModelBase() = default;

    virtual void initialize() { }

    std::string m_path;
    Material m_material;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    // Destruction through RAII
    VertexBuffer m_vertexBuffer;
    IndexBuffer<uint32_t> m_indexBuffer;
    UniformBuffer m_uniformBuffers;

    // No destruction necessary
    DescriptorSet m_descriptorSets;
  };

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
    virtual ~GeometryNodeBase() = default;

    std::shared_ptr<ModelBase> m_model;
    Material m_material;
  };
}

namespace std
{
  template<> struct hash<RX::ModelBase>
  {
    size_t operator()(const std::shared_ptr<RX::ModelBase> model) const { return hash<std::string>()(model->m_path); }
  };

  template<> struct hash<RX::Texture>
  {
    size_t operator()(const std::shared_ptr<RX::Texture> texture) const { return hash<std::string>()(texture->getInfo().path); }
  };
}

#endif // BASE_HPP