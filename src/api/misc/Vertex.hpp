#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "pch/stdafx.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace rx
{
  struct Vertex
  {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;

    static std::array<vk::VertexInputBindingDescription, 1> getBindingDescriptions()
    {
      std::array<vk::VertexInputBindingDescription, 1> bindindDescriptions{ };

      bindindDescriptions[0].binding = 0;
      bindindDescriptions[0].stride = sizeof(Vertex);
      bindindDescriptions[0].inputRate = vk::VertexInputRate::eVertex;

      return bindindDescriptions;
    }

    static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions()
    {
      std::array<vk::VertexInputAttributeDescription, 4> attributeDescriptions{ };

      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
      attributeDescriptions[0].offset = offsetof(Vertex, pos);

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
      attributeDescriptions[1].offset = offsetof(Vertex, color);

      attributeDescriptions[2].binding = 0;
      attributeDescriptions[2].location = 2;
      attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
      attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

      attributeDescriptions[3].binding = 0;
      attributeDescriptions[3].location = 3;
      attributeDescriptions[3].format = vk::Format::eR32G32B32Sfloat;
      attributeDescriptions[3].offset = offsetof(Vertex, normal);

      return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const
    {
      return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
    }
  };
}

namespace std
{
  template<> struct hash<rx::Vertex>
  {
    size_t operator()(const rx::Vertex& vertex) const
    {
      size_t hashPos = hash<glm::vec3>()(vertex.pos);
      size_t hashColor = hash<glm::vec3>()(vertex.color);
      size_t hashTexCoord = hash<glm::vec2>()(vertex.texCoord);
      size_t hashNormal = hash<glm::vec3>()(vertex.normal);

      return ((((hashPos ^ (hashColor << 1)) >> 1) ^ hashTexCoord) << 1) ^ hashNormal;
    }
  };
}

#endif // VERTEX_HPP