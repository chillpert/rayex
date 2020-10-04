#include "Settings.hpp"

#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  Settings::Settings( )
  {
    this->assetsPath = std::filesystem::current_path( ).string( ) += "/";

    std::replace( this->assetsPath.begin( ), this->assetsPath.end( ), '\\', '/' );

    g_assetsPath = this->assetsPath;
    RX_INFO( "Using default path as path to resources: ", this->assetsPath );
  }

  void Settings::setRecursionDepth( uint32_t recursionDepth )
  {
    if ( recursionDepth <= maxRecursionDepth )
    {
      this->recursionDepth = recursionDepth;
    }
    else
    {
      this->recursionDepth = this->maxRecursionDepth;
      RX_WARN( "Exceeded maximum recursion depth of ", this->maxRecursionDepth, ". Using highest possible value instead." );
    }

    this->refreshPipeline = true;
  }

  void Settings::setClearColor( const glm::vec4& clearColor )
  {
    this->clearColor       = clearColor;
    this->refreshSwapchain = true;
  }

  void Settings::setAssetsPath( int argc, char* argv[] )
  {
    this->assetsPath = "";

    for ( int i = 0; i < argc; ++i )
    {
      this->assetsPath += argv[i];
    }

    std::replace( this->assetsPath.begin( ), this->assetsPath.end( ), '\\', '/' );

    this->assetsPath = this->assetsPath.substr( 0, this->assetsPath.find_last_of( '/' ) + 1 );

    g_assetsPath = this->assetsPath;
  }

  void Settings::setAssetsPath( std::string_view path )
  {
    this->assetsPath = path;

    std::replace( this->assetsPath.begin( ), this->assetsPath.end( ), '\\', '/' );

    if ( path[path.size( ) - 1] != '/' )
    {
      this->assetsPath += '/';
    }

    g_assetsPath = this->assetsPath;
  }

  void Settings::setEnableRayTracing( bool flag )
  {
    this->rayTrace         = flag;
    this->refreshSwapchain = true;
  }

  void Settings::setAutomaticPipelineRefresh( bool flag )
  {
    this->automaticPipelineRefresh = flag;
  }

  void Settings::setMaxDirectionalLights( uint32_t amount )
  {
    if ( amount == 0 )
    {
      ++amount;
      RX_WARN( "Can not use value 0 for the amount of maximum directional lights. Using 1 instead." );
    }
    this->maxDirectionalLights = amount;
    this->refreshPipeline      = true;
  }

  void Settings::setMaxPointLights( uint32_t amount )
  {
    if ( amount == 0 )
    {
      ++amount;
      RX_WARN( "Can not use value 0 for the amount of maximum point lights. Using 1 instead." );
    }
    this->maxPointLights  = amount;
    this->refreshPipeline = true;
  }

  void Settings::setMaxGeometryNodes( uint32_t amount )
  {
    if ( amount == 0 )
    {
      ++amount;
      RX_WARN( "Can not use value 0 for the amount of maximum directional lights. Using 1 instead." );
    }
    this->maxGeometryNodes = amount;
  }
} // namespace RAYEXEC_NAMESPACE