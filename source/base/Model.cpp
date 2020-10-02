#include "base/Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace RAYEXEC_NAMESPACE
{
  uint32_t Model::modelCounter = 0;

  Model::Model( ) :
    index( rx::Model::modelCounter++ ) {}

  Model::Model( std::string_view path ) :
    index( rx::Model::modelCounter++ ),
    path( path )
  {
    load( );
  }

  void Model::load( )
  {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    std::string fullPath = g_resourcePath + this->path;

    if ( !tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, fullPath.c_str( ) ) )
    {
      RX_ERROR( warn + err );
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices;

    for ( const auto& shape : shapes )
    {
      for ( const auto& index : shape.mesh.indices )
      {
        Vertex vertex { };

        vertex.pos.x = attrib.vertices[3 * index.vertex_index + 0];
        vertex.pos.y = attrib.vertices[3 * index.vertex_index + 1];
        vertex.pos.z = attrib.vertices[3 * index.vertex_index + 2];

        vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
        vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
        vertex.normal.z = attrib.normals[3 * index.normal_index + 2];

        vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
        vertex.texCoord.y = 1.0F - attrib.texcoords[2 * index.texcoord_index + 1];

        vertex.color.x = 1.0F;
        vertex.color.y = 1.0F;
        vertex.color.z = 1.0F;

        if ( uniqueVertices.count( vertex ) == 0 )
        {
          uniqueVertices[vertex] = static_cast<uint32_t>( this->vertices.size( ) );
          this->vertices.push_back( vertex );
        }

        this->indices.push_back( uniqueVertices[vertex] );
      }
    }
  }

  auto Model::isLoaded( ) -> bool
  {
    if ( !this->vertices.empty( ) )
    {
      return true;
    }

    return false;
  }
} // namespace RAYEXEC_NAMESPACE