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
    path( path ) {}

  float getTextureIndex( std::string_view texturePath, std::vector<Texture>& textures )
  {
    if ( !texturePath.empty( ) )
    {
      // Check if texture already exists and set its index.
      // @note For testing purposes currently disabled.
      /*
      for ( size_t i = 0; i < textures.size( ); ++i )
      {
        if ( textures[i].getPath( ) == texturePath )
        {
          return static_cast<float>( i );
        }
      }
      */

      // Create a new texture
      textures.push_back( Texture( texturePath, true ) );
      return static_cast<float>( textures.size( ) - 1 );
    }

    // If the material does not have a texture, return -1.0F.
    return -1.0F;
  }

  void Model::load( std::vector<Texture>& textures )
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

    bool firstRun      = true;
    size_t prevOffsets = 0;

    // Loop over shapes
    for ( size_t s = 0; s < shapes.size( ); s++ )
    {
      Mesh mesh        = { };
      mesh.indexOffset = prevOffsets + shapes[s].mesh.num_face_vertices.size( );
      prevOffsets += mesh.indexOffset;

      int materialIndex = shapes[s].mesh.material_ids[0];
      if ( materialIndex != -1 )
      {
        Material mat = { };
        float z      = -1.0F;

        auto ambient = materials[materialIndex].ambient;
        mat.ambient  = glm::vec4( ambient[0], ambient[1], ambient[2], getTextureIndex( materials[materialIndex].ambient_texname, textures ) );

        auto diffuse = materials[materialIndex].diffuse;
        mat.diffuse  = glm::vec4( diffuse[0], diffuse[1], diffuse[2], getTextureIndex( materials[materialIndex].diffuse_texname, textures ) );

        auto specular = materials[materialIndex].specular;
        mat.specular  = glm::vec4( specular[0], specular[1], specular[2], getTextureIndex( materials[materialIndex].specular_texname, textures ) );

        mesh.material = mat;
      }

      this->meshes.push_back( mesh );

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
      }
    }

    RX_VERBOSE( "Loaded model from ", this->path, "\nMesh count: ", this->meshes.size( ) );
  }

  auto Model::isLoaded( ) -> bool
  {
    return !this->vertices.empty( );
  }
} // namespace RAYEXEC_NAMESPACE
