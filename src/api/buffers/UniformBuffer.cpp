#include "UniformBuffer.hpp"
#include "window/Time.hpp"

#include <glm/gtx/string_cast.hpp>

namespace RX
{
  std::vector<vk::Buffer> UniformBuffer::getRaw()
  {
    std::vector<vk::Buffer> res;
    for (Buffer& buffer : m_buffers)
      res.push_back(buffer.get());
    
    return res;
  }

  void UniformBuffer::init(UniformBufferInfo& info)
  {
    m_info = info;

    // Set up the staging buffer.
    BufferCreateInfo createInfo{ };
    createInfo.device = m_info.device;
    createInfo.physicalDevice = m_info.physicalDevice;
    createInfo.size = sizeof(UniformBufferObject);
    createInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer;
    createInfo.sharingMode = vk::SharingMode::eExclusive;
    createInfo.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    
    m_buffers.resize(m_info.swapchainImagesCount);

    for (Buffer& buffer : m_buffers)
      buffer.init(createInfo);
  }

  void UniformBuffer::destroy()
  {
    for (Buffer& buffer : m_buffers)
      buffer.destroy();
  }

  void UniformBuffer::upload(uint32_t imageIndex, UniformBufferObject& ubo)
  {
    m_buffers[imageIndex].fill<UniformBufferObject>(&ubo);
  }
}