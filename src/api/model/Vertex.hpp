#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct Vertex
  {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static std::array<VkVertexInputBindingDescription, 1> getBindingDescriptions()
    {
      std::array<VkVertexInputBindingDescription, 1> bindindDescriptions{ };

      bindindDescriptions[0].binding = 0;
      bindindDescriptions[0].stride = sizeof(Vertex);
      bindindDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return bindindDescriptions;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
      std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{ };

      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, pos);

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, color);

      attributeDescriptions[2].binding = 0;
      attributeDescriptions[2].location = 2;
      attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

      return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const
    {
      return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
  };
}

#endif // VERTEX_HPP