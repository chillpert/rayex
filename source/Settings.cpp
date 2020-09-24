#include "Settings.hpp"
#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  Settings::Settings( )
  {
    this->resourcePath = std::filesystem::current_path( ).string( ) += "/";

    std::replace( this->resourcePath.begin( ), this->resourcePath.end( ), '\\', '/' );

    g_resourcePath = this->resourcePath;
    RX_INFO( "Using default path as path to resources: ", this->resourcePath );
  }

  void Settings::setMaxRecursionDepth( uint32_t maxRecursionDepth )
  {
    this->maxRecursionDepth = maxRecursionDepth;
    this->refreshPipeline = true;
  }

  void Settings::setClearColor( const glm::vec4& clearColor )
  {
    this->clearColor = clearColor;
    this->refreshSwapchain = true;
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

    std::replace( this->resourcePath.begin( ), this->resourcePath.end( ), '\\', '/' );

    if ( path[path.size( ) - 1] != '/' )
      this->resourcePath += '/';

    g_resourcePath = this->resourcePath;
  }

  void Settings::setEnableRayTracing( bool flag )
  {
    this->rayTrace = flag;
    this->refreshSwapchain = true;
  }

  void Settings::setAutomaticPipelineRefresh( bool flag )
  {
    this->automaticPipelineRefresh = flag;
  }

  void Settings::setAnticipatedDirectionalLights( uint32_t amount )
  {
    this->anticipatedDirectionalLights = amount;
    this->refreshPipeline = true;
  }

  void Settings::setAnticipatedPointLights( uint32_t amount )
  {
    this->anticipatedPointLights = amount;
    this->refreshPipeline = true;
  }
}