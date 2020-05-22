#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Buffer.hpp"
#include "Vertex.hpp"
#include "api/Queues.hpp"

namespace RX
{
  class VertexBuffer
  {
  public:
    inline uint32_t getVertexCount() const { return static_cast<uint32_t>(m_buffer.getSize()); }
    inline VkBuffer get() const { return m_buffer.get(); }

    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, Queues& queue, std::vector<Vertex>& vertices);

  private:
    Buffer m_buffer;
  };
}

#endif // VERTEX_BUFFER_HPP