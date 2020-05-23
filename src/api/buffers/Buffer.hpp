#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "api/BaseComponent.hpp"

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
    VkIndexType type; // Optional, if the buffer is not an index buffer.
  };

  class Buffer : public BaseComponent
  {
  public:
    Buffer();
    ~Buffer();

    inline VkBuffer get() const { return m_buffer; }
    inline VkDeviceMemory getMemory() const { return m_memory; }

    void create(BufferCreateInfo& createInfo);

    Buffer& operator=(const Buffer& buffer);
    void copyTo(const Buffer& buffer) const;

  private:
    void destroy();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;

  public:
    BufferCreateInfo m_info;
  };
}

#endif // BUFFER_HPP