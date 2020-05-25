#ifndef UNIFORM_BUFFERS_HPP
#define UNIFORM_BUFFERS_HPP

#include "Buffer.hpp"

namespace RX
{
  struct UniformBufferObject
  {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
  };

  class UniformBuffers
  {
  public:
    inline std::vector<Buffer>& get() { return m_buffers; }

    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, size_t swapchainImagesCount, std::shared_ptr<UniformBufferObject> uniformBufferObject);

    void upload(uint32_t imageIndex);
    void destroy();

  private:
    std::shared_ptr<UniformBufferObject> m_ubo;
    std::vector<Buffer> m_buffers;
    VkDevice m_device;

    // The extent of the swapchain.
    float m_width;
    float m_height;
  };
}

#endif // UNIFORM_BUFFER_HPP