#ifndef MODEL_BASE_HPP
#define MODEL_BASE_HPP

#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "DescriptorSet.hpp"
#include "Shader.hpp" // TODO: this is probably not needed here.

namespace RX
{
  class Material
  {
  public:
    Material() = default;
    Material(const std::string& diffuseTexture) :
      m_diffuseTexture(diffuseTexture) { }

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    glm::vec3 m_transmittance;
    glm::vec3 m_emission;

    float m_shininess;
    float m_reflectionIndex;
    float m_opacity;

    std::string m_ambientTexture;
    std::string m_diffuseTexture;
    std::string m_specularTexture;
    std::string m_bumpTexture;
    std::string m_displacementTexture;
    std::string m_alphaTexture;
    std::string m_reflectionTexture;

    std::unordered_set<std::string> getTextures()
    {
      std::unordered_set<std::string> textures;

      if (!m_ambientTexture.empty())
        textures.insert(m_ambientTexture);

      if (!m_diffuseTexture.empty())
        textures.insert(m_diffuseTexture);

      if (!m_specularTexture.empty())
        textures.insert(m_specularTexture);

      if (!m_bumpTexture.empty())
        textures.insert(m_bumpTexture);

      if (!m_displacementTexture.empty())
        textures.insert(m_displacementTexture);

      if (!m_alphaTexture.empty())
        textures.insert(m_alphaTexture);

      if (!m_reflectionTexture.empty())
        textures.insert(m_reflectionTexture);

      return textures;
    }
  };

  class ModelBase
  {
  public:
    ModelBase(const std::string& path) :
      m_path(path) { }

    virtual ~ModelBase() = default;

    virtual void load() = 0;

    bool isLoaded()
    {
      if (m_vertices.size() > 0)
        return true;

      return false;
    }

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
}

namespace std
{
  template<> struct hash<RX::ModelBase>
  {
    size_t operator()(const std::shared_ptr<RX::ModelBase> model) const { return hash<std::string>()(model->m_path); }
  };
}

#endif // MODEL_BASE_HPP