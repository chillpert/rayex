#include "base/Geometry.hpp"

#include "api/Components.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyObj/tiny_obj_loader.h"

namespace RAYEX_NAMESPACE
{
  std::shared_ptr<Geometry> loadObj( std::string_view path, bool dynamic )
  {
    RX_LOG_TIME_START( "Loading model from ", path, "." );

    std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>( );
    geometry->path                     = path;
    geometry->geometryIndex            = components::geometryIndex++;
    geometry->dynamic                  = dynamic;

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
    int i = 0;
    for ( const auto& shape : shapes )
    {
      Mesh mesh            = { };
      auto numFaceVertices = static_cast<uint32_t>( shape.mesh.num_face_vertices.size( ) );
      mesh.indexOffset     = prevOffsets + numFaceVertices;
      prevOffsets += numFaceVertices;

      int materialIndex = shape.mesh.material_ids[0];
      if ( materialIndex != -1 )
      {
        float z = -1.0F;

        auto diffuse     = materials[materialIndex].diffuse;
        mesh.material.kd = glm::vec3( diffuse[0], diffuse[1], diffuse[2] );

        auto emission          = materials[materialIndex].emission;
        mesh.material.emission = glm::vec3( emission[0], emission[1], emission[2] );

        mesh.material.illum = static_cast<uint32_t>( materials[materialIndex].illum );
        mesh.material.d     = materials[materialIndex].dissolve;
        //mesh.material.ns    = materials[materialIndex].shininess;
        mesh.material.ni = materials[materialIndex].ior;

        //auto transmittance          = materials[materialIndex].transmittance;
        //mesh.material.transmittance = glm::vec3( transmittance[0], transmittance[1], transmittance[2] );

        // @todo Add relative path here instead of inside the .mtl file.
        mesh.material.diffuseTexPath = materials[materialIndex].diffuse_texname;
      }

      geometry->meshes.push_back( mesh );

      for ( const auto& index : shape.mesh.indices )
      {
        Vertex vertex = { };
        if ( attrib.vertices.size( ) > 3 * index.vertex_index + 0 )
        {
          vertex.pos.x = attrib.vertices[3 * index.vertex_index + 0];
          vertex.pos.y = attrib.vertices[3 * index.vertex_index + 1];
          vertex.pos.z = attrib.vertices[3 * index.vertex_index + 2];
        }

        if ( attrib.normals.size( ) > 3 * index.normal_index + 0 )
        {
          vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
          vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
          vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
        }

        if ( attrib.texcoords.size( ) > 2 * index.texcoord_index + 1 )
        {
          vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
          vertex.texCoord.y = 1.0F - attrib.texcoords[2 * index.texcoord_index + 1];
        }

        if ( attrib.colors.size( ) > 3 * index.vertex_index + 2 )
        {
          vertex.color.x = attrib.colors[3 * index.vertex_index + 0];
          vertex.color.y = attrib.colors[3 * index.vertex_index + 1];
          vertex.color.z = attrib.colors[3 * index.vertex_index + 2];
        }

        if ( uniqueVertices.count( vertex ) == 0 )
        {
          uniqueVertices[vertex] = static_cast<uint32_t>( geometry->vertices.size( ) );
          geometry->vertices.push_back( vertex );
        }

        geometry->indices.push_back( uniqueVertices[vertex] );
      }
    }

    RX_LOG_TIME_STOP( "Loaded ", geometry->meshes.size( ), " sub-mesh(es)" );

    return std::move( geometry );
  }

  void Geometry::setMaterial( const Material& material )
  {
    for ( Mesh& mesh : meshes )
    {
      mesh.material = material;
    }
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

  void GeometryInstance::setTransform( const glm::mat4& transform )
  {
    this->transform = transform;
    transformIT     = glm::transpose( glm::inverse( transform ) );
  }
} // namespace RAYEX_NAMESPACE
