#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "Buffer.hpp"

namespace RX
{
  template <typename T = uint32_t>
  struct IndexBufferInfo
  {
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::CommandPool commandPool;
    vk::Queue queue;
    std::vector<uint32_t> queueIndices;
    std::vector<T> indices;
  };

  template <typename T = uint32_t>
  class IndexBuffer
  {
  public:
    inline vk::Buffer get() const { return m_buffer.get(); }
    inline uint32_t getCount() { return m_count; }
    inline vk::IndexType getType() { return m_indexType; }

    void initialize(IndexBufferInfo<T>& info);
  
  private:
    Buffer m_buffer;
    IndexBufferInfo<T> m_info;

    uint32_t m_count;
    vk::IndexType m_indexType;
  };

  template <typename T>
  void IndexBuffer<T>::initialize(IndexBufferInfo<T>& info)
  {
    m_info = info;
    m_count = static_cast<uint32_t>(m_info.indices.size());

    if (typeid(T) == typeid(uint8_t))
      m_indexType = vk::IndexType::eUint8EXT;

    else if (typeid(T) == typeid(uint16_t))
      m_indexType = vk::IndexType::eUint16;

    else if (typeid(T) == typeid(uint32_t))
      m_indexType = vk::IndexType::eUint32;

    else
      RX_ERROR("Invalid data type for index buffer was specified.");

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = m_info.physicalDevice;
    stagingInfo.device = m_info.device;
    stagingInfo.size = sizeof(m_info.indices[0]) * m_info.indices.size();
    stagingInfo.usage = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    stagingInfo.sharingMode = vk::SharingMode::eConcurrent;
    stagingInfo.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    stagingInfo.commandPool = m_info.commandPool;
    
    vk::MemoryAllocateFlagsInfo allocateFlags(vk::MemoryAllocateFlagBitsKHR::eDeviceAddress);
    stagingInfo.pNextMemory = &allocateFlags;
    stagingInfo.queue = m_info.queue;
    stagingInfo.queueFamilyIndices = m_info.queueIndices;

    // Set up the actual index buffer.
    BufferCreateInfo bufferInfo = stagingInfo;
    bufferInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    bufferInfo.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;

    Buffer stagingBuffer(stagingInfo);
    stagingBuffer.fill<T>(m_info.indices.data());

    m_buffer.initialize(bufferInfo);

    // Copy staging buffer to the actual index buffer.
    m_buffer = stagingBuffer;
  }
}

#endif // INDEX_BUFFER_HPP