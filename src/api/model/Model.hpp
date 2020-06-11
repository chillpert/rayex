#ifndef MODEL_HPP
#define MODEL_HPP

#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSetLayout.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace RX
{
  struct Material
  {
    glm::fvec3 ambient;
    glm::fvec3 diffuse;
    glm::fvec3 specular;
    glm::fvec3 transmittance;
    glm::fvec3 emission;
    float shininess;
    float reflectionIndex;
    float opacity;

    std::string ambientTexture;
    std::string diffuseTexture;
    std::string specularTexture;
    std::string bumpTexture;
    std::string displacementTexture;
    std::string alphaTexture;
    std::string reflectionATexture;
  };

  class Model
  {
  public:
    RX_API ~Model();

    // Fill the vectors with vertex and index data.
    RX_API virtual void initialize();

    inline UniformBufferObject getUbo() { return { m_model, m_view, m_projection }; }

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    std::string m_pathToTexture;
    std::string m_pathToModel;

    // Destruction through RAII
    VertexBuffer m_vertexBuffer;
    IndexBuffer<> m_indexBuffer;
    // TODO: do not store the texture in here like this. Use the strings from materials. 
    Texture m_texture; 
    UniformBuffer m_uniformBuffers;

    glm::mat4 m_model = glm::mat4(1.0f);
    glm::mat4 m_inversedModel = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_projection = glm::mat4(1.0f);
    glm::vec3 m_cameraPos = glm::vec3(1.0f);

    uint32_t txtOffset = 0; // Offset in 'm_textures'

    std::vector<Material> m_materials; // This probably requires keeping track of submeshes.

    // No destruction necessary
    DescriptorSet m_descriptorSets;
  };
}

#endif // MODEL_HPP