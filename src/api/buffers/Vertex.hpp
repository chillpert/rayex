#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct Vertex
  {
    glm::vec2 pos;
    glm::vec3 color;

    static std::array<VkVertexInputBindingDescription, 1> getBindingDescriptions()
    {
      std::array<VkVertexInputBindingDescription, 1> description{ };
      description[0].binding = 0;
      description[0].stride = sizeof(Vertex);
      description[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

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
}

#endif // VERTEX_HPP