#ifndef MODEL_HPP
#define MODEL_HPP

#include "api/buffers/Vertex.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/UniformBuffers.hpp"
#include "api/buffers/DescriptorSets.hpp"
#include "api/buffers/DescriptorPool.hpp"
#include "api/buffers/DescriptorSetLayout.hpp"
#include "api/Shader.hpp"
#include "api/texture/Texture.hpp"

namespace RX
{
  class Model
  {
  public:
    RX_API Model() = default;
    RX_API Model(const std::string& pathToModel) :
      m_pathToModel(pathToModel) { }

    RX_API virtual ~Model() = default;

    // Fill the vectors with vertex and index data
    RX_API virtual void initialize();

    inline UniformBufferObject getUbo() { return { m_model, m_view, m_projection }; }

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    std::string m_pathToTexture;
    std::string m_pathToModel;

    // Destruction through RAII
    Texture m_texture;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    UniformBuffers m_uniformBuffers;
    DescriptorPool m_descriptorPool;

    glm::mat4 m_model = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_projection = glm::mat4(1.0f);

    // No destruction necessary
    DescriptorSets m_descriptorSets;
  };
}

#endif // MODEL_HPP