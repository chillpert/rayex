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

    inline uint32_t getCount() const { return m_buffer.m_info.count; }
    inline uint32_t getDeviceSize() const { return static_cast<uint32_t>(m_buffer.m_info.deviceSize); }

    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<Vertex>& vertices);

  private:
    Buffer m_buffer;
  };
}

#endif // VERTEX_BUFFER_HPP