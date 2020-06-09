#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "Buffer.hpp"

namespace RX
{
  template <typename T = uint32_t>
  struct IndexBufferInfo
  {
    std::vector<T> indices;
    vk::Device device;
    vk::PhysicalDevice physicalDevice;
    vk::CommandPool commandPool;
    vk::Queue queue;
    std::vector<uint32_t> queueIndices;
  };

  template <typename T = uint32_t>
  class IndexBuffer
  {
  public:
    inline vk::Buffer get() const { return m_buffer.get(); }
    inline uint32_t getCount() { return m_buffer.getInfo().count; }
    inline vk::IndexType getType() { return m_buffer.getInfo().type; }
    inline IndexBufferInfo<T> getInfo() { return m_info; }

    void initialize(IndexBufferInfo<T>& info);
  
  private:
    Buffer m_buffer;
    IndexBufferInfo<T> m_info;
  };

  template <typename T>
  void IndexBuffer<T>::initialize(IndexBufferInfo<T>& info)
  {
    m_info = info;

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = m_info.physicalDevice;
    stagingInfo.device = m_info.device;
    stagingInfo.deviceSize = sizeof(m_info.indices[0]) * m_info.indices.size();
    stagingInfo.count = static_cast<uint32_t>(m_info.indices.size());
    stagingInfo.usage = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    stagingInfo.sharingMode = vk::SharingMode::eConcurrent;
    stagingInfo.properties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    stagingInfo.commandPool = m_info.commandPool;
    stagingInfo.componentName = "index staging buffer";

    vk::MemoryAllocateFlagsInfo allocateFlags;
    allocateFlags.flags = vk::MemoryAllocateFlagBitsKHR::eDeviceAddress;

    stagingInfo.pNextMemory = &allocateFlags;
    
    stagingInfo.queue = m_info.queue;
    stagingInfo.queueFamilyIndexCount = static_cast<uint32_t>(m_info.queueIndices.size());
    stagingInfo.queueFamilyIndices = m_info.queueIndices;

    // Set up the actual index buffer.
    BufferCreateInfo bufferInfo = stagingInfo;
    bufferInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    bufferInfo.properties = vk::MemoryPropertyFlagBits::eDeviceLocal;
    bufferInfo.componentName = "index buffer";

    if (typeid(T) == typeid(uint8_t))
      bufferInfo.type = vk::IndexType::eUint8EXT;

    else if (typeid(T) == typeid(uint16_t))
      bufferInfo.type = vk::IndexType::eUint16;

    else if (typeid(T) == typeid(uint32_t))
      bufferInfo.type = vk::IndexType::eUint32;

    else
      RX_ERROR("Invalid data type for index buffer was specified.");

    Buffer stagingBuffer;
    stagingBuffer.initialize(stagingInfo);
    stagingBuffer.fill<T>(m_info.indices.data());

    m_buffer.initialize(bufferInfo);

    // Copy staging buffer to the actual index buffer.
    m_buffer = stagingBuffer;
  }
}

#endif // INDEX_BUFFER_HPP