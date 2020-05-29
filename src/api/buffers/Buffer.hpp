#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Image.hpp"

namespace RX
{
  // Custom struct for saving information related to buffer creation.
  struct BufferCreateInfo
  {
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkDeviceSize deviceSize; // The size required for the buffer.
    uint32_t count; // The amount of elements in the received vector.
    VkBufferUsageFlags usage;
    VkSharingMode sharingMode;
    VkMemoryPropertyFlags properties;
    VkCommandPool commandPool;
    VkQueue queue;
    uint32_t queueFamilyIndexCount; // Optional, if sharing mode is concurrent.
    const uint32_t* pQueueFamilyIndices; // Optional, if sharing mode is concurrent.

    VkIndexType type; // Ignore, will be filled automatically.
  };

  class Buffer
  {
  public:
    RX_API ~Buffer();

    inline VkBuffer get() const { return m_buffer; }
    inline VkDeviceMemory getMemory() { return m_memory; }
    inline VkDeviceSize getSize() const { return m_info.deviceSize; }
    inline BufferCreateInfo& getInfo() { return m_info; }

    void initialize(BufferCreateInfo& createInfo);

    template <class T>
    void fill(T* source);

    // Is associated with the copyToBuffer function.
    //Buffer& operator()(const Buffer& buffer);
    Buffer& operator=(const Buffer& buffer);
    void copyToBuffer(const Buffer& buffer) const;

    void copyToImage(Image& image) const;

    void destroy();

    static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
  
  private:
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;
    BufferCreateInfo m_info;

    bool m_created = false;
  };

  template <class T>
  inline void Buffer::fill(T* source)
  {
    void* data;
    vkMapMemory(m_info.device, m_memory, 0, m_info.deviceSize, 0, &data);
    memcpy(data, source, static_cast<uint32_t>(m_info.deviceSize));
    vkUnmapMemory(m_info.device, m_memory);
  }
}

#endif // BUFFER_HPP