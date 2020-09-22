#include "base/Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace RENDERER_NAMESPACE
{
  uint32_t Model::modelCounter = 0;

  Model::Model( ) :
    index( this->modelCounter++ ) { }

  Model::Model( const std::string& path ) :
    index( this->modelCounter++ ), path( path )
  {
    load( );
  }

  void Model::load( )
  {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string fullPath = g_resourcePath + this->path;

    if ( !tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, fullPath.c_str( ) ) )
      RX_ERROR( warn + err );

    std::unordered_map<Vertex, uint32_t> uniqueVertices;

    for ( const auto& shape : shapes )
    {
      for ( const auto& index : shape.mesh.indices )
      {
        Vertex vertex { };

        vertex.pos =
        {
          attrib.vertices[3 * index.vertex_index + 0],
          attrib.vertices[3 * index.vertex_index + 1],
          attrib.vertices[3 * index.vertex_index + 2]
        };

        vertex.normal =
        {
          attrib.normals[3 * index.normal_index + 0],
          attrib.normals[3 * index.normal_index + 1],
          attrib.normals[3 * index.normal_index + 2]
        };

        /*
        vertex.texCoord =
        {
          attrib.texcoords[2 * index.texcoord_index + 0];,
          1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
        };
        */

        vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
        vertex.texCoord.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];

        vertex.color = { 1.0f, 1.0f, 1.0f };

        if ( uniqueVertices.count( vertex ) == 0 )
        {
          uniqueVertices[vertex] = static_cast<uint32_t>( this->vertices.size( ) );
          this->vertices.push_back( vertex );
        }

        this->indices.push_back( uniqueVertices[vertex] );
      }
    }
  }

  bool Model::isLoaded( )
  {
    if ( this->vertices.size( ) > 0 )
      return true;

    return false;
  }
}