#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include "Buffer.hpp"

namespace RX
{
  struct UniformBufferObject
  {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::vec3 cameraPos = glm::vec3(1.0f);
  };

  struct UniformBufferInfo
  {
    //UniformBufferObject uniformBufferObject;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    size_t swapchainImagesCount;
  };

  class UniformBuffer
  {
  public:
    inline std::vector<Buffer>& get() { return m_buffers; }
    std::vector<vk::Buffer> getRaw();
    inline UniformBufferInfo& getInfo() { return m_info; }

    void initialize(UniformBufferInfo& info);
    void destroy();

    void upload(uint32_t imageIndex, UniformBufferObject& ubo);

  private:
    std::vector<Buffer> m_buffers;
    UniformBufferInfo m_info;
  };
}

#endif // UNIFORM_BUFFER_HPP