#include "Settings.hpp"

namespace RENDERER_NAMESPACE
{
  uint32_t Settings::s_maxRecursionDepth = 8;
  bool Settings::s_refresh = false;
  glm::vec4 Settings::s_clearColor = glm::vec4( 0.4f, 0.4f, 0.4f, 1.0f );
  std::string Settings::s_resourcePath;

  void Settings::setResourcePath( int argc, char* argv[] )
  {
    s_resourcePath = "";

    for ( int i = 0; i < argc; ++i)
      s_resourcePath += argv[i];

    s_resourcePath = s_resourcePath.substr( 0, s_resourcePath.find_last_of('/') + 1 );    
  }

  void Settings::setResourcePath( const std::string& path )
  {
    s_resourcePath = path;

    if ( path[ path.size( ) - 1] != '/' )
     s_resourcePath += '/';
  }
}