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
  
    BufferCreateInfo m_info;
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;
  };
}

#endif