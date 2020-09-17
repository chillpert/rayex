#include "Settings.hpp"
#include "api/misc/Components.hpp"

namespace RENDERER_NAMESPACE
{
  Settings::Settings( )
  {
    m_resourcePath = std::filesystem::current_path( ).string( ) += "/";
    g_resourcePath = m_resourcePath;
    RX_INFO( "Using default path as path to resouces: ", m_resourcePath );
  }

  Settings::Settings( const std::string& test ) :
    test( test ) {  }

  void Settings::setMaxRecursionDepth( uint32_t maxRecursionDepth, bool refresh )
  {
    m_maxRecursionDepth = maxRecursionDepth;
    m_refresh = refresh;
  }

  void Settings::setClearColor( const glm::vec4& clearColor, bool refresh )
  {
    m_clearColor = clearColor;
    m_refresh = refresh;
  }

  void Settings::setResourcePath( int argc, char* argv[] )
  {
    m_resourcePath = "";

    for ( int i = 0; i < argc; ++i)
      m_resourcePath += argv[i];

    std::replace( m_resourcePath.begin( ), m_resourcePath.end( ), '\\', '/' );

    m_resourcePath = m_resourcePath.substr( 0, m_resourcePath.find_last_of('/') + 1 );    

    g_resourcePath = m_resourcePath;
  }

  void Settings::setResourcePath( const std::string& path )
  {
    m_resourcePath = path;

    std::replace( m_resourcePath.begin( ), m_resourcePath.end( ), '\\', '/');

    if ( path[ path.size( ) - 1] != '/' )
      m_resourcePath += '/';

    g_resourcePath = m_resourcePath;
  }
}