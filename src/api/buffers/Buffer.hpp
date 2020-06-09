#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Image.hpp"

namespace RX
{
  // Custom struct for saving information related to buffer creation.
  struct BufferCreateInfo
  {
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::DeviceSize deviceSize; // The size required for the buffer.
    uint32_t count; // The amount of elements in the received vector.
    vk::BufferUsageFlags usage;
    vk::SharingMode sharingMode;
    vk::MemoryPropertyFlags properties;
    vk::CommandPool commandPool;
    vk::Queue queue;
    uint32_t queueFamilyIndexCount = UINT32_MAX; // Optional, if sharing mode is not concurrent.
    std::vector<uint32_t> queueFamilyIndices; // Optional, if sharing mode is not concurrent.
    void* pNextMemory; // Ignore, if buffer was not set with BufferUsageFlagBits::eShaderDeviceAddressKHR

    const char* componentName = "buffer"; // Optional, if you happen to use the default buffer for another purpose you can give it a better name. This will only effect logging output.
    vk::IndexType type; // Ignore, will be filled automatically.
  };

  class Buffer
  {
  public:
    RX_API ~Buffer();

    inline vk::Buffer get() const { return m_buffer; }
    inline vk::DeviceMemory& getMemory() { return m_memory; }
    inline vk::DeviceSize getSize() const { return m_info.deviceSize; }
    inline BufferCreateInfo& getInfo() { return m_info; }

    void initialize(BufferCreateInfo& createInfo);

    template <class T>
    void fill(T* source);

    // Is associated with the copyToBuffer function.
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
    vkMapMemory(m_info.device, m_memory, 0, m_info.deviceSize, 0, &data);
    memcpy(data, source, static_cast<uint32_t>(m_info.deviceSize));
    vkUnmapMemory(m_info.device, m_memory);
  }
}

#endif // BUFFER_HPP