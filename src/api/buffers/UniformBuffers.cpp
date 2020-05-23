#include "UniformBuffers.hpp"
#include "window/Time.hpp"

namespace RX
{
  void UniformBuffers::initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, size_t swapchainImagesCount)
  {
    m_device = device;
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

  /*
  This probably requires major refactoring so that it can be used from the outside.
  Or better indirectly via some interface. Well, who knows yet?
  */
  void UniformBuffers::update(uint32_t imageIndex)
  {
    static float startTime = Time::getTime();

    float currentTime = Time::getTime();
    float time = currentTime - startTime;

    UniformBufferObject ubo{ };
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.projection = glm::perspective(glm::radians(45.0f), m_width / m_height, 0.1f, 10.0f);
    ubo.projection[1][1] *= -1;

    void* data;
    vkMapMemory(m_device, m_buffers[imageIndex].getMemory(), 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(m_device, m_buffers[imageIndex].getMemory());
  }

  void UniformBuffers::render()
  {

  }

  void UniformBuffers::destroy()
  {
    for (auto& it : m_buffers)
      it.destroy();
  }
}