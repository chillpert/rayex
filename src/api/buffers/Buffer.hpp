#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "InitializerInfos.hpp"
#include "Image.hpp"

namespace RX
{
  class Buffer
  {
  public:
    Buffer() = default;
    Buffer(BufferCreateInfo& createInfo);
    RX_API ~Buffer();

    Buffer& operator=(const Buffer& buffer);
    void copyToBuffer(const Buffer& buffer) const;
    void copyToImage(Image& image) const;

    inline vk::Buffer get() const { return m_buffer; }
    inline vk::DeviceMemory& getMemory() { return m_memory; }
    inline vk::DeviceSize getSize() const { return m_info.size; }

    void init(BufferCreateInfo& createInfo);

    template <class T>
    void fill(T* source);

    RX_API void destroy();

  private:
    vk::Buffer m_buffer;
    vk::DeviceMemory m_memory;
    BufferCreateInfo m_info;
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