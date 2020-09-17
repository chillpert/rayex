#include "Settings.hpp"
#include "api/misc/Components.hpp"

namespace RENDERER_NAMESPACE
{
  Settings::Settings( )
  {
    this->resourcePath = std::filesystem::current_path( ).string( ) += "/";
    g_resourcePath = this->resourcePath;
    RX_INFO( "Using default path as path to resouces: ", this->resourcePath );
  }

  Settings::Settings( const std::string& test ) :
    test( test ) {  }

  void Settings::setMaxRecursionDepth( uint32_t maxRecursionDepth, bool refresh )
  {
    this->maxRecursionDepth = maxRecursionDepth;
    this->refresh = refresh;
  }

  void Settings::setClearColor( const glm::vec4& clearColor, bool refresh )
  {
    this->clearColor = clearColor;
    this->refresh = refresh;
  }

  void Settings::setResourcePath( int argc, char* argv[] )
  {
    this->resourcePath = "";

    for ( int i = 0; i < argc; ++i)
      this->resourcePath += argv[i];

    std::replace( this->resourcePath.begin( ), this->resourcePath.end( ), '\\', '/' );

    this->resourcePath = this->resourcePath.substr( 0, this->resourcePath.find_last_of('/') + 1 );    

    g_resourcePath = this->resourcePath;
  }

  void Settings::setResourcePath( const std::string& path )
  {
    this->resourcePath = path;

    std::replace( this->resourcePath.begin( ), this->resourcePath.end( ), '\\', '/');

    if ( path[ path.size( ) - 1] != '/' )
      this->resourcePath += '/';

    g_resourcePath = this->resourcePath;
  }
}