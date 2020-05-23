#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "BufferWrapper.hpp"
#include "Vertex.hpp"
#include "api/Queues.hpp"

namespace RX
{
  class Buffer
  {
  public:
    inline VkBuffer get() const { return m_buffer.get(); }
    inline uint32_t getCount() const { return m_buffer.m_info.entryCount; }
    inline VkIndexType getType() const { return m_type; }
    inline uint32_t getDeviceSize() const { return static_cast<uint32_t>(m_buffer.m_info.deviceSize); }

    template <class T>
    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, Queues& queue, std::vector<T>& vertices, VkBufferUsageFlags usage);

  private:
    BufferWrapper m_buffer;
    VkIndexType m_type;
  };

  template<class T>
  inline void Buffer::initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, Queues& queue, std::vector<T>& vertices, VkBufferUsageFlags usage)
  {
    if (typeid(T) == typeid(uint8_t))
      m_type = VK_INDEX_TYPE_UINT8_EXT;

    else if (typeid(T) == typeid(uint16_t))
      m_type = VK_INDEX_TYPE_UINT16;

    else if (typeid(T) == typeid(uint32_t))
      m_type = VK_INDEX_TYPE_UINT32;

    
    VkDeviceSize size = sizeof(vertices[0]) * vertices.size();

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = physicalDevice;
    stagingInfo.device = device;
    stagingInfo.deviceSize = size;
    stagingInfo.entryCount = static_cast<uint32_t>(vertices.size());
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingInfo.commandPool = commandPool;
    stagingInfo.queue = queue.getGraphicsQueue();

    BufferWrapper stagingBuffer;
    stagingBuffer.create(stagingInfo);

    void* data;
    vkMapMemory(device, stagingBuffer.getMemory(), 0, size, 0, &data);
    memcpy(data, vertices.data(), static_cast<uint32_t>(size));
    vkUnmapMemory(device, stagingBuffer.getMemory());

    // Set up the actual vertex buffer.
    BufferCreateInfo bufferInfo = stagingInfo;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
    bufferInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    m_buffer.create(bufferInfo);

    // Copy staging buffer to the actual vertex buffer.
    m_buffer = stagingBuffer;
  }
}

#endif // BUFFER_HPP