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
    RX_API Model()
    {
      ubo = std::make_shared<UniformBufferObject>();
    }

    // Fill the vectors with vertex and index data
    void load();

    // Destruction through RAII
    Texture texture;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    UniformBuffers uniformBuffers;
    DescriptorPool descriptorPool;

    // No destruction necessary
    DescriptorSets descriptorSets;
    std::shared_ptr<UniformBufferObject> ubo;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::string texturePath;
    std::string modelPath;
  };
}

#endif // MODEL_HPP