#ifndef UNIFORM_BUFFERS_HPP
#define UNIFORM_BUFFERS_HPP

#include "Buffer.hpp"

namespace RX
{
  struct UniformBufferObject
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
  };

  class UniformBuffers
  {
  public:
    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, size_t swapchainImagesCount);
    void update(uint32_t imageIndex);
    void render();

  private:
    std::vector<Buffer> m_buffers;
    VkDevice m_device;

    // The extent of the swapchain.
    float m_width;
    float m_height;
  };
}

#endif // UNIFORM_BUFFER_HPP