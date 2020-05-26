#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Buffer.hpp"
#include "Vertex.hpp"

namespace RX
{
  class VertexBuffer
  {
  public:
    inline VkBuffer get() const { return m_buffer.get(); }
    inline VkDeviceSize getSize() const { return m_buffer.getSize(); }

    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<Vertex>& vertices);

  private:
    Buffer m_buffer;
  };
}

#endif // VERTEX_BUFFER_HPP