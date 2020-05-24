#include "UniformBuffers.hpp"
#include "window/Time.hpp"

#include <glm/gtx/string_cast.hpp>

namespace RX
{
  void UniformBuffers::initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, size_t swapchainImagesCount, std::shared_ptr<UniformBufferObject> uniformBufferObject)
  {
    m_device = device;
    m_ubo = uniformBufferObject;
    m_width = static_cast<float>(extent.width);
    m_height = static_cast<float>(extent.height);

    // Set up the staging buffer.
    BufferCreateInfo createInfo{ };
    createInfo.device = device;
    createInfo.physicalDevice = physicalDevice;
    createInfo.deviceSize = sizeof(UniformBufferObject);
    createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    m_buffers.resize(swapchainImagesCount);

    for (auto& it : m_buffers)
      it.create(createInfo);
  }

  void UniformBuffers::upload(uint32_t imageIndex)
  {
    UniformBufferObject temp = (*m_ubo.get());

    void* data;
    vkMapMemory(m_device, m_buffers[imageIndex].getMemory(), 0, sizeof(temp), 0, &data);

    memcpy(data, &temp, sizeof(temp));
    vkUnmapMemory(m_device, m_buffers[imageIndex].getMemory());
  }

  void UniformBuffers::destroy()
  {
    for (auto& it : m_buffers)
      it.destroy();
  }
}