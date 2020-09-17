#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// Contains all rendering properties and textures.
  class Material
  {
  public:
    Material( ) = default;
    Material( const std::string& diffuseTexture ) :
      diffuseTexture( { diffuseTexture } )
    { }

    glm::vec3 ambient = { };
    glm::vec3 diffuse = { };
    glm::vec3 specular = { };
    glm::vec3 transmittance = { };
    glm::vec3 emission = { };

    float shininess = 1.0f;
    float reflectionIndex = 1.0f;
    float opacity = 1.0f;

    std::vector<std::string> diffuseTexture;
    std::vector<std::string> specularTexture;
    std::vector<std::string> bumpTexture;
    std::vector<std::string> displacementTexture;
    std::vector<std::string> alphaTexture;
    std::vector<std::string> reflectionTexture;

    /// @return Returns a set of all textures this model requires.
    std::unordered_set<std::string> getTextures( )
    {
      std::unordered_set<std::string> textures;

      if ( !diffuseTexture.empty( ) )
        textures.insert( diffuseTexture.begin( ), diffuseTexture.end( ) );

      if ( !specularTexture.empty( ) )
        textures.insert( specularTexture.begin( ), specularTexture.end( ) );

      if ( !bumpTexture.empty( ) )
        textures.insert( bumpTexture.begin( ), bumpTexture.end( ) );

      if ( !displacementTexture.empty( ) )
        textures.insert( displacementTexture.begin( ), displacementTexture.end( ) );

      if ( !alphaTexture.empty( ) )
        textures.insert( alphaTexture.begin( ), alphaTexture.end( ) );

      if ( !reflectionTexture.empty( ) )
        textures.insert( reflectionTexture.begin( ), reflectionTexture.end( ) );

      return textures;
    }
  };
}

#endif // MAterial_HPP