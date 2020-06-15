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
}

namespace std
{
  template<> struct hash<RX::ModelBase>
  {
    size_t operator()(const std::shared_ptr<RX::ModelBase> model) const { return hash<std::string>()(model->m_path); }
  };
}

#endif // MODEL_BASE_HPP