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
  class Model
  {
  public:
    RX_API Model() = default;
    RX_API virtual ~Model();

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
    Texture m_texture;
    DescriptorPool m_descriptorPool;
    UniformBuffer m_uniformBuffers;

    glm::mat4 m_model = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_projection = glm::mat4(1.0f);

    // No destruction necessary
    DescriptorSet m_descriptorSets;
  };
}

#endif // MODEL_HPP