#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "stdafx.hpp"

namespace rx
{
  /// Contains all rendering properties and textures.
  class Material
  {
  public:
    Material( ) = default;
    Material( const std::string& diffuseTexture ) :
      m_diffuseTexture( { diffuseTexture } )
    { }

    glm::vec3 m_ambient = { };
    glm::vec3 m_diffuse = { };
    glm::vec3 m_specular = { };
    glm::vec3 m_transmittance = { };
    glm::vec3 m_emission = { };

    float m_shininess = 1.0f;
    float m_reflectionIndex = 1.0f;
    float m_opacity = 1.0f;

    std::vector<std::string> m_diffuseTexture;
    std::vector<std::string> m_specularTexture;
    std::vector<std::string> m_bumpTexture;
    std::vector<std::string> m_displacementTexture;
    std::vector<std::string> m_alphaTexture;
    std::vector<std::string> m_reflectionTexture;

    /// @return Returns a set of all textures this model requires.
    std::unordered_set<std::string> getTextures( )
    {
      std::unordered_set<std::string> textures;

      if ( !m_diffuseTexture.empty( ) )
        textures.insert( m_diffuseTexture.begin( ), m_diffuseTexture.end( ) );

      if ( !m_specularTexture.empty( ) )
        textures.insert( m_specularTexture.begin( ), m_specularTexture.end( ) );

      if ( !m_bumpTexture.empty( ) )
        textures.insert( m_bumpTexture.begin( ), m_bumpTexture.end( ) );

      if ( !m_displacementTexture.empty( ) )
        textures.insert( m_displacementTexture.begin( ), m_displacementTexture.end( ) );

      if ( !m_alphaTexture.empty( ) )
        textures.insert( m_alphaTexture.begin( ), m_alphaTexture.end( ) );

      if ( !m_reflectionTexture.empty( ) )
        textures.insert( m_reflectionTexture.begin( ), m_reflectionTexture.end( ) );

      return textures;
    }
  };
}

#endif // MAterial_HPP