#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "api/BaseComponent.hpp"

namespace RX
{
  class Buffer : public BaseComponent
  {
  public:
    Buffer(const char* componentName);
    virtual ~Buffer() = default;

    inline VkBuffer get() { return m_buffer; }
    
  protected:
    void create(VkBufferUsageFlags usage, VkSharingMode sharingMode);
    void allocate(VkMemoryPropertyFlags properties);
    void bind();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    VkDeviceSize m_size;
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;

    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
  };
}

#endif