#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  struct BufferCreateInfo
  {
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkSharingMode sharingMode;
    VkMemoryPropertyFlags properties;
    VkCommandPool commandPool;
    VkQueue queue;
    uint32_t queueFamilyIndexCount;
    const uint32_t* pQueueFamilyIndices;
  };

  class Buffer : public BaseComponent
  {
  public:
    Buffer();
    ~Buffer();

    inline VkBuffer get() const { return m_buffer; }
    inline VkDeviceMemory getMemory() const { return m_memory; }
    inline VkDeviceSize getSize() const { return m_info.size; }

    void create(BufferCreateInfo& createInfo);

    Buffer& operator=(const Buffer& buffer);
    void copyTo(const Buffer& buffer) const;

  private:
    void destroy();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  
    BufferCreateInfo m_info;
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;
  };
}

#endif