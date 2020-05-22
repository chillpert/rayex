#include "VertexBuffer.hpp"
#include "Vertex.hpp"

namespace RX
{
  VertexBuffer::VertexBuffer() :
    Buffer("VertexBuffer") { }

  VertexBuffer::~VertexBuffer()
  {
    destroy();
  }

  void VertexBuffer::initialize(VkDevice device, VkPhysicalDevice physicalDevice, std::vector<Vertex>& vertices)
  {
    m_device = device;
    m_physicalDevice = physicalDevice;

    auto iter = std::next(vertices.begin(), vertices.size());
    std::move(vertices.begin(), iter, std::back_inserter(m_vertices));
    vertices.erase(vertices.begin(), iter);

    m_size = sizeof(m_vertices[0]) * m_vertices.size(); // TODO: this should not be hard-coded obviously

    create(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
    allocate(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    bind();
    
    // Fill the buffer.
    void* data;
    vkMapMemory(device, m_memory, 0, m_size, 0, &data);
    memcpy(data, vertices.data(), m_size);
    vkUnmapMemory(device, m_memory);

    initializationCallback();
  }

  void VertexBuffer::destroy()
  {
    assertDestruction();
    vkDestroyBuffer(m_device, m_buffer, nullptr);
    vkFreeMemory(m_device, m_memory, nullptr);
  }
}