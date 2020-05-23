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

    inline VkIndexType getType() const { return m_buffer.m_info.type; }
    inline uint32_t getCount() const { return m_buffer.m_info.count; }
    inline uint32_t getDeviceSize() const { return static_cast<uint32_t>(m_buffer.m_info.deviceSize); }

    // This will setup the buffer in a way that works with the given input. 
    // The process is based on the data type of the vector that has been passed to this function.
    template <class T>
    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<T>& vertices);
    // TODO: if a queue with Transfer Bit only is available utilize it. Otherwise fall back to just using the graphics queue.

  private:
    BufferWrapper m_buffer;
  };

  template<class T>
  inline void Buffer::initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<T>& vertices)
  {
    VkDeviceSize size = sizeof(vertices[0]) * vertices.size();

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = physicalDevice;
    stagingInfo.device = device;
    stagingInfo.deviceSize = size;
    stagingInfo.count = static_cast<uint32_t>(vertices.size());
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingInfo.commandPool = commandPool;
    stagingInfo.queue = queue;

    BufferWrapper stagingBuffer;
    stagingBuffer.create(stagingInfo);

    void* data;
    vkMapMemory(device, stagingBuffer.getMemory(), 0, size, 0, &data);
    memcpy(data, vertices.data(), static_cast<uint32_t>(size));
    vkUnmapMemory(device, stagingBuffer.getMemory());

    // Set up the actual vertex buffer.
    BufferCreateInfo bufferInfo = stagingInfo;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    // Every incoming data type must be supported by the implementation.
    if (typeid(T) == typeid(uint8_t))
    {
      bufferInfo.type = VK_INDEX_TYPE_UINT8_EXT;
      bufferInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    else if (typeid(T) == typeid(uint16_t))
    {
      bufferInfo.type = VK_INDEX_TYPE_UINT16;
      bufferInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    else if (typeid(T) == typeid(uint32_t))
    {
      bufferInfo.type = VK_INDEX_TYPE_UINT32;
      bufferInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    else if (typeid(T) == typeid(Vertex))
    {
      bufferInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    else
    {
      RX_ERROR("Could not deduce buffer data input type.");
    }

    m_buffer.create(bufferInfo);

    // Copy staging buffer to the actual vertex buffer.
    m_buffer = stagingBuffer;
  }
}

#endif // BUFFER_HPP