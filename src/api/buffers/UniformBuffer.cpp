#include "UniformBuffer.hpp"
#include "window/Time.hpp"

#include <glm/gtx/string_cast.hpp>

namespace RX
{
  UniformBuffer::~UniformBuffer()
  {
    destroy();
  }

  std::vector<VkBuffer> UniformBuffer::getRaw()
  {
    std::vector<VkBuffer> res;
    for (Buffer& buffer : m_buffers)
      res.push_back(buffer.get());
    
    return res;
  }

  void UniformBuffer::initialize(UniformBufferInfo& info)
  {
    m_info = info;

    // Set up the staging buffer.
    BufferCreateInfo createInfo{ };
    createInfo.device = m_info.device;
    createInfo.physicalDevice = m_info.physicalDevice;
    createInfo.deviceSize = sizeof(UniformBufferObject);
    createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    m_buffers.resize(m_info.swapchainImagesCount);

    for (auto& it : m_buffers)
      it.initialize(createInfo);
  }

  void UniformBuffer::upload(uint32_t imageIndex, UniformBufferObject& ubo)
  {
    m_buffers[imageIndex].fill<UniformBufferObject>(&ubo);
  }

  void UniformBuffer::destroy()
  {
    for (auto& it : m_buffers)
      it.destroy();
  }
}