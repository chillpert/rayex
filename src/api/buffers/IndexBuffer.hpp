#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "Buffer.hpp"

namespace RX
{
  template <typename T = uint32_t>
  struct IndexBufferInfo
  {
    std::vector<T> indices;
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkCommandPool commandPool;
    VkQueue queue;
  };

  template <typename T = uint32_t>
  class IndexBuffer
  {
  public:
    inline VkBuffer get() const { return m_buffer.get(); }
    inline uint32_t getCount() const { return m_buffer.m_info.count; }
    inline VkIndexType getType() const { return m_buffer.m_info.type; }
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
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingInfo.commandPool = m_info.commandPool;
    stagingInfo.queue = m_info.queue;

    // Set up the actual index buffer.
    BufferCreateInfo bufferInfo = stagingInfo;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufferInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if (typeid(T) == typeid(uint8_t))
      bufferInfo.type = VK_INDEX_TYPE_UINT8_EXT;

    else if (typeid(T) == typeid(uint16_t))
      bufferInfo.type = VK_INDEX_TYPE_UINT16;

    else if (typeid(T) == typeid(uint32_t))
      bufferInfo.type = VK_INDEX_TYPE_UINT32;

    else
      RX_ERROR("Invalid data type for index buffer was specified.");

    Buffer stagingBuffer;
    stagingBuffer.initialize(stagingInfo);
    stagingBuffer.fill<T>(m_info.indices.data());

    m_buffer.initialize(bufferInfo);

    // Copy staging buffer to the actual index buffer.
    m_buffer(stagingBuffer);
  }
}

#endif // INDEX_BUFFER_HPP