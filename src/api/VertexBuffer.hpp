#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "BaseComponent.hpp"

namespace RX
{
  struct Vertex
  {
    static const uint32_t attributeCount = 2;

    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription()
    {
      VkVertexInputBindingDescription description{ };
      description.binding = 0;
      description.stride = sizeof(Vertex);
      description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return description;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
    {
      std::array<VkVertexInputAttributeDescription, 2> description{ };

      description[0].binding = 0;
      description[0].location = 0;
      description[0].format = VK_FORMAT_R32G32_SFLOAT;
      description[0].offset = offsetof(Vertex, pos);

      description[1].binding = 0;
      description[1].location = 1;
      description[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      description[1].offset = offsetof(Vertex, color);

      return description;
    }
  };
  
  class VertexBuffer : public BaseComponent
  {
  public:
    VertexBuffer();
    ~VertexBuffer();

    inline VkBuffer get() { return m_buffer; }

    void initialize(VkDevice device, VkPhysicalDevice physicalDevice);
    void destroy();

  private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkBuffer m_buffer;
    VkDeviceMemory m_memory;

    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
  };

  // Triangle :D
  const std::vector<Vertex> vertices =
  {
    {{0.0f, -0.5f}, {0.8f, 0.0f, 0.3f}},
    {{0.5f, 0.5f}, {0.3f, 0.1f, 0.0f}},
    {{-0.5f, 0.5f}, {0.5f, 0.1f, 0.7f}}
  };
}

#endif // VERTEX_BUFFER_HPP