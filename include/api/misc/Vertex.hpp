#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "pch/stdafx.hpp"

#define GLENABLE_EXPERIMENTAL
#include "external/glm/gtx/hash.hpp"

namespace RENDERER_NAMESPACE
{
  /// Describes a vertex and its data.
  /// @ingroup API
  struct Vertex
  {
    glm::vec3 pos; ///< The vertex's position in 3D space.
    glm::vec3 color; ///< The vertex's color.
    glm::vec2 texCoord; ///< The vertex's texture coordinate.
    glm::vec3 normal; ///< The vertex's normal vector.

    /// @return Returns the hard-coded vertex format (eR32G32B32Sfloat).
    inline static vk::Format getVertexFormat( ) { return vk::Format::eR32G32B32Sfloat; }

    /// @return Returns the vertex's Vulkan binding description.
    static std::array<vk::VertexInputBindingDescription, 1> getBindingDescriptions( )
    {
      std::array<vk::VertexInputBindingDescription, 1> bindingDescriptions { };

      bindingDescriptions[0].binding = 0;
      bindingDescriptions[0].stride = sizeof( Vertex );
      bindingDescriptions[0].inputRate = vk::VertexInputRate::eVertex;

      return bindingDescriptions;
    }

    /// @return Returns the vertex's Vulkan attribute descriptions for all four attributes.
    static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions( )
    {
      std::array<vk::VertexInputAttributeDescription, 4> attributeDescriptions { };

      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = getVertexFormat( );
      attributeDescriptions[0].offset = offsetof( Vertex, pos );

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
      attributeDescriptions[1].offset = offsetof( Vertex, color );

      attributeDescriptions[2].binding = 0;
      attributeDescriptions[2].location = 2;
      attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
      attributeDescriptions[2].offset = offsetof( Vertex, texCoord );

      attributeDescriptions[3].binding = 0;
      attributeDescriptions[3].location = 3;
      attributeDescriptions[3].format = vk::Format::eR32G32B32Sfloat;
      attributeDescriptions[3].offset = offsetof( Vertex, normal );

      return attributeDescriptions;
    }

    /// @cond INTERNAL
    bool operator==( const Vertex& other ) const
    {
      return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
    }
    /// @endcond
  };
}

namespace std
{
  /// @cond INTERNAL
  template<> struct hash<rx::Vertex>
  {
    size_t operator()( const rx::Vertex& vertex ) const
    {
      size_t hashPos = hash<glm::vec3>( )( vertex.pos );
      size_t hashColor = hash<glm::vec3>( )( vertex.color );
      size_t hashTexCoord = hash<glm::vec2>( )( vertex.texCoord );
      size_t hashNormal = hash<glm::vec3>( )( vertex.normal );

      return ( ( ( ( hashPos ^ ( hashColor << 1 ) ) >> 1 ) ^ hashTexCoord ) << 1 ) ^ hashNormal;
    }
  };
  /// @endcond
}

#endif // VERTEX_HPP