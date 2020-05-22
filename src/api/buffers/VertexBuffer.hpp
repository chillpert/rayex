#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Buffer.hpp"
#include "Vertex.hpp"

namespace RX
{
  class VertexBuffer
  {
  public:
    inline uint32_t getVertexCount() { return static_cast<uint32_t>(m_vertices.size()); }
    inline VkBuffer get() { return m_buffer.get(); }

    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<Vertex>& vertices);

  private:
    std::vector<Vertex> m_vertices;

    Buffer m_buffer;
  };
}

#endif // VERTEX_BUFFER_HPP