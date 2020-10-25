#include "base/Geometry.hpp"

#include "api/Components.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace RAYEX_NAMESPACE
{
  std::shared_ptr<Geometry> loadObj( std::string_view path )
  {
    RX_LOG_TIME_START( "Loading model from ", path, "." );

    std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>( );
    geometry->path                     = path;
    geometry->geometryIndex            = components::geometryIndex++;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool res = tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, std::string( path ).c_str( ) );

    if ( !warn.empty( ) )
    {
      RX_WARN( warn );
    }

    if ( !err.empty( ) )
    {
      RX_FATAL( "Failed to load model from ", path, "\nERROR: ", err );
    }

    if ( !res )
    {
      RX_FATAL( "Failed to load model from ", path );
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices;
    geometry->meshes.reserve( shapes.size( ) );

    bool firstRun        = true;
    uint32_t prevOffsets = 0;

    // Loop over shapes.
    for ( size_t s = 0; s < shapes.size( ); s++ )
    {
      Mesh mesh        = { };
      mesh.indexOffset = prevOffsets + static_cast<uint32_t>( shapes[s].mesh.num_face_vertices.size( ) );
      prevOffsets += mesh.indexOffset;

      int materialIndex = shapes[s].mesh.material_ids[0];
      if ( materialIndex != -1 )
      {
        Material mat = { };
        float z      = -1.0F;

        auto ambient = materials[materialIndex].ambient;
        mat.ambient  = glm::vec3( ambient[0], ambient[1], ambient[2] );

        auto diffuse = materials[materialIndex].diffuse;
        mat.diffuse  = glm::vec3( diffuse[0], diffuse[1], diffuse[2] );

        auto specular = materials[materialIndex].specular;
        mat.specular  = glm::vec3( specular[0], specular[1], specular[2] );

        mesh.material = mat;

        // @todo Add relative path here instead of inside the .mtl file.
        mesh.material.diffuseTexPath = materials[materialIndex].diffuse_texname;
      }

      geometry->meshes.push_back( mesh );

      // Loop over faces.
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

          if ( attrib.texcoords.size( ) > 2 * idx.texcoord_index + 1 )
          {
            vertex.texCoord.x = attrib.texcoords[2 * idx.texcoord_index + 0];
            vertex.texCoord.y = 1.0F - attrib.texcoords[2 * idx.texcoord_index + 1];
          }

          if ( attrib.colors.size( ) > 3 * idx.vertex_index + 2 )
          {
            vertex.color.x = attrib.colors[3 * idx.vertex_index + 0];
            vertex.color.y = attrib.colors[3 * idx.vertex_index + 1];
            vertex.color.z = attrib.colors[3 * idx.vertex_index + 2];
          }

          if ( uniqueVertices.count( vertex ) == 0 )
          {
            uniqueVertices[vertex] = static_cast<uint32_t>( geometry->vertices.size( ) );
            geometry->vertices.push_back( vertex );
          }

          geometry->indices.push_back( uniqueVertices[vertex] );
        }

        index_offset += fv;
      }
    }

    RX_LOG_TIME_STOP( "Loaded ", geometry->meshes.size( ), " sub-mesh(es)" );

    return std::move( geometry );
  }

  std::shared_ptr<GeometryInstance> instance( std::shared_ptr<Geometry> geometry, const glm::mat4& transform )
  {
    RX_ASSERT( geometry != nullptr, "Invalid geometry." );

    std::shared_ptr<GeometryInstance> result = std::make_shared<GeometryInstance>( );
    result->geometryIndex                    = geometry->geometryIndex;
    result->transform                        = transform;
    result->transformIT                      = glm::transpose( glm::inverse( transform ) );

    return std::move( result );
  }
} // namespace RAYEX_NAMESPACE
