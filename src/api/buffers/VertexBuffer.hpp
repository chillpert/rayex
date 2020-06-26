#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "Buffer.hpp"
#include "Vertex.hpp"

namespace RX
{
  struct VertexBufferInfo
  {
    std::vector<Vertex> vertices;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::CommandPool commandPool;
    vk::Queue queue;
    std::vector<uint32_t> queueIndices;
  };

  class VertexBuffer
  {
  public:
    inline vk::Buffer get() { return m_buffer.get(); }
    inline vk::DeviceSize getSize() const { return m_buffer.getSize(); }
    inline uint32_t getCount() const { return m_count; }

    void init(VertexBufferInfo& info);

  private:
    Buffer m_buffer;
    VertexBufferInfo m_info;

    uint32_t m_count;
  };
}

#endif // VERTEX_BUFFER_HPP