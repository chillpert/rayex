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

    bool res = tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, this->path.c_str( ) );

    if ( !warn.empty( ) )
    {
      RX_WARN( warn );
    }

    if ( !err.empty( ) )
    {
      RX_FATAL( "Failed to load model from ", this->path, "\nERROR: ", err );
    }

    if ( !res )
    {
      RX_FATAL( "Failed to load model from ", this->path );
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices;
    this->meshes.reserve( shapes.size( ) );

    // Loop over shapes
    for ( size_t s = 0; s < shapes.size( ); s++ )
    {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      for ( size_t f = 0; f < shapes[s].mesh.num_face_vertices.size( ); f++ )
      {
        int fv = shapes[s].mesh.num_face_vertices[f];

        // Loop over vertices in the face.
        for ( size_t v = 0; v < fv; v++ )
        {
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

          Vertex vertex = { };
          if ( attrib.vertices.size( ) > 3 * idx.vertex_index + 0 )
          {
            vertex.pos.x = attrib.vertices[3 * idx.vertex_index + 0];
            vertex.pos.y = attrib.vertices[3 * idx.vertex_index + 1];
            vertex.pos.z = attrib.vertices[3 * idx.vertex_index + 2];
          }

          if ( attrib.normals.size( ) > 3 * idx.normal_index + 0 )
          {
            vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
            vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
            vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
          }

          if ( attrib.colors.size( ) > 2 * idx.texcoord_index + 1 )
          {
            vertex.texCoord.x = attrib.texcoords[2 * idx.texcoord_index + 0];
            vertex.texCoord.y = attrib.texcoords[2 * idx.texcoord_index + 1];
          }

          if ( attrib.colors.size( ) > 3 * idx.vertex_index + 2 )
          {
            vertex.color.x = attrib.colors[3 * idx.vertex_index + 0];
            vertex.color.y = attrib.colors[3 * idx.vertex_index + 1];
            vertex.color.z = attrib.colors[3 * idx.vertex_index + 2];
          }

          if ( uniqueVertices.count( vertex ) == 0 )
          {
            uniqueVertices[vertex] = static_cast<uint32_t>( this->vertices.size( ) );
            this->vertices.push_back( vertex );
          }

          this->indices.push_back( uniqueVertices[vertex] );
        }
        index_offset += fv;

        // per-face material
        //shapes[s].mesh.material_ids[f];
      }
    }
  }

  auto Model::isLoaded( ) -> bool
  {
    return !this->vertices.empty( );
  }
} // namespace RAYEXEC_NAMESPACE
