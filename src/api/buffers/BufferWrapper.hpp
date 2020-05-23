#ifndef BUFFER_WRAPPER_HPP
#define BUFFER_WRAPPER_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  struct BufferCreateInfo
  {
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkDeviceSize deviceSize;
    uint32_t entryCount;
    VkBufferUsageFlags usage;
    VkSharingMode sharingMode;
    VkMemoryPropertyFlags properties;
    VkCommandPool commandPool;
    VkQueue queue;
    uint32_t queueFamilyIndexCount;
    const uint32_t* pQueueFamilyIndices;
  };

  class BufferWrapper : public BaseComponent
  {
  public:
    BufferWrapper();
    ~BufferWrapper();

    inline VkBuffer get() const { return m_buffer; }
    inline VkDeviceMemory getMemory() const { return m_memory; }

    void create(BufferCreateInfo& createInfo);

    BufferWrapper& operator=(const BufferWrapper& buffer);
    void copyTo(const BufferWrapper& buffer) const;

  private:
    void destroy();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;

  public:
    BufferCreateInfo m_info;
  };
}

#endif // BUFFER_WRAPPER_HPP