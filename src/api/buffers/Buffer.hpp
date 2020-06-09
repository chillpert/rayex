#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Image.hpp"

namespace RX
{
  struct BufferCreateInfo
  {
    // General information
    vk::PhysicalDevice physicalDevice;
    vk::Device device;

    // Staging
    vk::CommandPool stagingCommandPool; // Optional, if there is no staging buffer involved.
    vk::Queue stagingQueue; // Optional, if there is no staging buffer involved.

    // Buffer
    void* pNextBuffer = nullptr; // Optional
    vk::BufferCreateFlags bufferFlags; // Optional
    vk::DeviceSize size; // The size required for the buffer.
    vk::BufferUsageFlags usage;
    vk::SharingMode sharingMode;
    std::vector<uint32_t> queueFamilyIndices; // Optional, if sharing mode is not concurrent.

    // Memory
    vk::MemoryPropertyFlags memoryProperties;
    void* pNextMemory = nullptr; // Optional
    vk::DeviceSize memoryOffset = 0;
  };

  class Buffer
  {
  public:
    RX_API ~Buffer();

    inline vk::Buffer get() const { return m_buffer; }
    inline vk::DeviceMemory& getMemory() { return m_memory; }
    inline vk::DeviceSize getSize() const { return m_info.size; }
    inline BufferCreateInfo& getInfo() { return m_info; }

    void initialize(BufferCreateInfo& createInfo);

    template <class T>
    void fill(T* source);

    Buffer& operator=(const Buffer& buffer);
    void copyToBuffer(const Buffer& buffer) const;

    void copyToImage(Image& image) const;

    RX_API void destroy();

    static uint32_t findMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);
  
  private:
    vk::Buffer m_buffer;
    vk::DeviceMemory m_memory;
    BufferCreateInfo m_info;

    bool m_created = false;
    bool m_allocated = false;
  };

  template <class T>
  inline void Buffer::fill(T* source)
  {
    void* data;
    vkMapMemory(m_info.device, m_memory, 0, m_info.size, 0, &data);
    memcpy(data, source, static_cast<uint32_t>(m_info.size));
    vkUnmapMemory(m_info.device, m_memory);
  }
}

#endif // BUFFER_HPP