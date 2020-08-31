#ifndef MODEL_BASE_HPP
#define MODEL_BASE_HPP

#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "DescriptorSet.hpp"

namespace rx
{
  class Material
  {
  public:
    Material( ) = default;
    Material( const std::string& diffuseTexture ) :
      m_diffuseTexture( diffuseTexture )
    { }

    glm::vec3 m_ambient = glm::vec3( 0.0f );
    glm::vec3 m_diffuse = glm::vec3( 0.0f );
    glm::vec3 m_specular = glm::vec3( 0.0f );
    glm::vec3 m_transmittance = glm::vec3( 0.0f );
    glm::vec3 m_emission = glm::vec3( 0.0f );

    float m_shininess = 1.0f;
    float m_reflectionIndex = 1.0f;
    float m_opacity = 1.0f;

    std::string m_ambientTexture;
    std::string m_diffuseTexture;
    std::string m_specularTexture;
    std::string m_bumpTexture;
    std::string m_displacementTexture;
    std::string m_alphaTexture;
    std::string m_reflectionTexture;

    std::unordered_set<std::string> getTextures( )
    {
      std::unordered_set<std::string> textures;

      if ( !m_ambientTexture.empty( ) )
        textures.insert( m_ambientTexture );

      if ( !m_diffuseTexture.empty( ) )
        textures.insert( m_diffuseTexture );

      if ( !m_specularTexture.empty( ) )
        textures.insert( m_specularTexture );

      if ( !m_bumpTexture.empty( ) )
        textures.insert( m_bumpTexture );

      if ( !m_displacementTexture.empty( ) )
        textures.insert( m_displacementTexture );

      if ( !m_alphaTexture.empty( ) )
        textures.insert( m_alphaTexture );

      if ( !m_reflectionTexture.empty( ) )
        textures.insert( m_reflectionTexture );

      return textures;
    }
  };

  class Model
  {
  public:
    Model( ) = default;
    Model( const std::string& path );

    void load( );
    bool isLoaded( );

    std::string m_path;
    Material m_material;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    // Destruction through RAII
    VertexBuffer m_vertexBuffer;
    IndexBuffer<uint32_t> m_indexBuffer;

    // No destruction necessary
    DescriptorSet m_descriptorSets;

    uint32_t m_index = 0;

    bool m_initialized = false;
  };
}

namespace std
{
  template<> struct hash<rx::Model>
  {
    size_t operator()( const std::shared_ptr<rx::Model> model ) const { return hash<std::string>( )( model->m_path ); }
  };
}

#endif // MODEL_BASE_HPP