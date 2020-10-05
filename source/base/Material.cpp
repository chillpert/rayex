#include "base/Material.hpp"

namespace RAYEXEC_NAMESPACE
{
  Material::Material( std::string_view diffuseTexture ) :
    diffuseTextures( { std::string( diffuseTexture ) } )
  {
  }
} // namespace RAYEXEC_NAMESPACE