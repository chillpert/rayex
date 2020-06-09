#include "VertexBuffer.hpp"

namespace RX
{
  void VertexBuffer::initialize(VertexBufferInfo& info)
  {
    m_info = info;
    m_count = static_cast<uint32_t>(m_info.vertices.size());

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = m_info.physicalDevice;
    stagingInfo.device = m_info.device;
    stagingInfo.size = sizeof(m_info.vertices[0]) * m_info.vertices.size();
    stagingInfo.usage = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    stagingInfo.sharingMode = vk::SharingMode::eConcurrent;
    stagingInfo.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    stagingInfo.stagingCommandPool = m_info.commandPool;
    
    stagingInfo.stagingQueue = m_info.queue;
    stagingInfo.queueFamilyIndices = m_info.queueIndices;

    vk::MemoryAllocateFlagsInfo allocateFlags;
    allocateFlags.flags = vk::MemoryAllocateFlagBitsKHR::eDeviceAddress;

    stagingInfo.pNextMemory = &allocateFlags;

    // Set up the actual index buffer.
    BufferCreateInfo bufferInfo = stagingInfo;
    bufferInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    bufferInfo.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;

    Buffer stagingBuffer;
    stagingBuffer.initialize(stagingInfo);
    stagingBuffer.fill<Vertex>(m_info.vertices.data());

    m_buffer.initialize(bufferInfo);

    // Copy staging buffer to the actual index buffer.
    m_buffer = stagingBuffer;
  }
}