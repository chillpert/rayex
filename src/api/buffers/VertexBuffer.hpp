#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Buffer.hpp"
#include "Vertex.hpp"

namespace RX
{
  struct VertexBufferInfo
  {
    std::vector<Vertex> vertices;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkCommandPool commandPool;
    VkQueue queue;
    std::vector<uint32_t> queueIndices;
  };

  class VertexBuffer
  {
  public:
    inline VkBuffer get()& { return m_buffer.get(); }
    inline VkDeviceSize getSize() const { return m_buffer.getSize(); }
    inline VertexBufferInfo& getInfo() { return m_info; }

    void initialize(VertexBufferInfo& info);

  private:
    Buffer m_buffer;
    VertexBufferInfo m_info;
  };
}

#endif // VERTEX_BUFFER_HPP