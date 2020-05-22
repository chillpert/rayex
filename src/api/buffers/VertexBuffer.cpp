#include "VertexBuffer.hpp"
#include "Vertex.hpp"

namespace RX
{
  void VertexBuffer::initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<Vertex>& vertices)
  {
    m_vertices.resize(vertices.size());
    std::move(vertices.begin(), vertices.end(), std::back_inserter(m_vertices));
    vertices.erase(vertices.begin(), vertices.end());
    vertices.clear();

    VkDeviceSize size = sizeof(m_vertices[0]) * m_vertices.size(); // TODO: this should not be hard-coded obviously

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = physicalDevice;
    stagingInfo.device = device;
    stagingInfo.size = size;
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingInfo.commandPool = commandPool;
    stagingInfo.queue = queue;

    Buffer stagingBuffer;
    stagingBuffer.create(stagingInfo);

    void* data;
    vkMapMemory(device, stagingBuffer.getMemory(), 0, size, 0, &data);
    memcpy(data, vertices.data(), static_cast<uint32_t>(size));
    vkUnmapMemory(device, stagingBuffer.getMemory());

    // Set up the actual vertex buffer.
    BufferCreateInfo bufferInfo{ };
    bufferInfo.physicalDevice = physicalDevice;
    bufferInfo.device = device;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    bufferInfo.commandPool = commandPool;
    bufferInfo.queue = queue;

    m_buffer.create(bufferInfo);

    // Copy staging buffer to the actual vertex buffer.
    m_buffer = stagingBuffer;
  }
}