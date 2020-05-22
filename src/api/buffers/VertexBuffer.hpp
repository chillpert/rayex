#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Buffer.hpp"
#include "Vertex.hpp"

namespace RX
{
  class VertexBuffer : public Buffer
  {
  public:
    VertexBuffer();
    ~VertexBuffer() override;

    inline uint32_t getVertexCount() { return static_cast<uint32_t>(m_vertices.size()); }

    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, std::vector<Vertex>& vertices);
    void destroy();

  private:
    std::vector<Vertex> m_vertices;
  };
}

#endif // VERTEX_BUFFER_HPP