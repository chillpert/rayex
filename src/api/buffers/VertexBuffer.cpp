#include "VertexBuffer.hpp"

namespace RX
{
  void VertexBuffer::initialize(VertexBufferInfo& info)
  {
    m_info = info;

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = m_info.physicalDevice;
    stagingInfo.device = m_info.device;
    stagingInfo.deviceSize = sizeof(m_info.vertices[0]) * m_info.vertices.size();
    stagingInfo.count = static_cast<uint32_t>(m_info.vertices.size());
    stagingInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
    stagingInfo.sharingMode = vk::SharingMode::eConcurrent;
    stagingInfo.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    stagingInfo.commandPool = m_info.commandPool;
    stagingInfo.componentName = "vertex staging buffer";

    stagingInfo.queue = m_info.queue;
    stagingInfo.queueFamilyIndexCount = static_cast<uint32_t>(m_info.queueIndices.size());
    stagingInfo.queueFamilyIndices = m_info.queueIndices;

    // Set up the actual index buffer.
    BufferCreateInfo bufferInfo = stagingInfo;
    bufferInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
    bufferInfo.properties = vk::MemoryPropertyFlagBits::eDeviceLocal;
    bufferInfo.componentName = "vertex buffer";

    Buffer stagingBuffer;
    stagingBuffer.initialize(stagingInfo);
    stagingBuffer.fill<Vertex>(m_info.vertices.data());
 
    m_buffer.initialize(bufferInfo);

    // Copy staging buffer to the actual index buffer.
    m_buffer = stagingBuffer;
  }
}