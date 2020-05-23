#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "Buffer.hpp"

namespace RX
{
  class IndexBuffer
  {
  public:
    inline VkBuffer get() const { return m_buffer.get(); }
    inline uint32_t getCount() const { return m_buffer.m_info.count; }
    inline VkIndexType getType() const { return m_buffer.m_info.type; }

    template <typename T>
    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<T>& indices);
  
  private:
    Buffer m_buffer;
  };

  template <typename T>
  void IndexBuffer::initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, std::vector<T>& indices)
  {
    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = physicalDevice;
    stagingInfo.device = device;
    stagingInfo.deviceSize = sizeof(indices[0]) * indices.size();
    stagingInfo.count = static_cast<uint32_t>(indices.size());
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingInfo.commandPool = commandPool;
    stagingInfo.queue = queue;

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
    stagingBuffer.create(stagingInfo);

    void* data;
    vkMapMemory(device, stagingBuffer.getMemory(), 0, stagingInfo.deviceSize, 0, &data);
    memcpy(data, indices.data(), static_cast<uint32_t>(stagingInfo.deviceSize));
    vkUnmapMemory(device, stagingBuffer.getMemory());

    m_buffer.create(bufferInfo);

    // Copy staging buffer to the actual index buffer.
    m_buffer = stagingBuffer;
  }
}

#endif // INDEX_BUFFER_HPP