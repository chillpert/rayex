#include "base/Settings.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  void Settings::setRecursionDepth( uint32_t recursionDepth )
  {
    if ( recursionDepth <= _maxRecursionDepth )
    {
      _recursionDepth = recursionDepth;
    }
    else
    {
      _recursionDepth = _maxRecursionDepth;
      RX_WARN( "Exceeded maximum recursion depth of ", _maxRecursionDepth, ". Using highest possible value instead." );
    }

    _refreshPipeline = true;
  }

  void Settings::setClearColor( const glm::vec4& clearColor )
  {
    _clearColor       = clearColor;
    _refreshSwapchain = true;

    components::frameCount = 0;
  }

  void Settings::setAssetsPath( int argc, char* argv[] )
  {
    _assetsPath = "";

    for ( int i = 0; i < argc; ++i )
    {
      _assetsPath += argv[i];
    }

    std::replace( _assetsPath.begin( ), _assetsPath.end( ), '\\', '/' );

    _assetsPath = _assetsPath.substr( 0, _assetsPath.find_last_of( '/' ) + 1 );

    components::assetsPath = _assetsPath;
  }

  void Settings::setAssetsPath( std::string_view path )
  {
    _assetsPath = path;

    std::replace( _assetsPath.begin( ), _assetsPath.end( ), '\\', '/' );

    if ( path[path.size( ) - 1] != '/' )
    {
      _assetsPath += '/';
    }

    components::assetsPath = _assetsPath;
  }

  void Settings::setAutomaticPipelineRefresh( bool flag )
  {
    _automaticPipelineRefresh = flag;
  }

  void Settings::setMaxDirectionalLights( uint32_t amount )
  {
    if ( amount == 0 )
    {
      ++amount;
      RX_WARN( "Can not use value 0 for the amount of maximum directional lights. Using 1 instead." );
    }
    _maxDirectionalLights = amount;
    _refreshPipeline      = true;
  }

  void Settings::setMaxPointLights( uint32_t amount )
  {
    if ( amount == 0 )
    {
      ++amount;
      RX_WARN( "Can not use value 0 for the amount of maximum point lights. Using 1 instead." );
    }
    _maxPointLights  = amount;
    _refreshPipeline = true;
  }

  void Settings::setMaxGeometryInstances( uint32_t amount )
  {
    if ( amount == 0 )
    {
      ++amount;
      RX_WARN( "Can not use value 0 for the amount of maximum directional lights. Using 1 instead." );
    }

    // Increment by one to accommodate the triangle dummy for emtpy scenes.
    _maxGeometryInstances = ++amount;
  }

  void Settings::setMaxGeoemtry( uint32_t amount )
  {
    if ( amount == 0 )
    {
      RX_WARN( "Can not use value 0 for the amount of maximum geometry. Using 16 instead." );
      amount = 16;
    }

    if ( amount % 4 != 0 )
    {
      RX_WARN( "Minimum storage buffer alignment must be a multiple of 4. Using 16 instead." );
      amount = 16;
    }

    _maxGeometry = amount;
  }

  void Settings::setDefaultAssetsPath( )
  {
    _assetsPath = std::filesystem::current_path( ).string( ) += "/";

    std::replace( _assetsPath.begin( ), _assetsPath.end( ), '\\', '/' );

    components::assetsPath = _assetsPath;
    RX_WARN( "No path to assets specified. Using default path as path to resources: ", _assetsPath );
  }

  void Settings::setPerPixelSampleRate( uint32_t sampleRate )
  {
    _perPixelSampleRate = sampleRate;
  }

  void Settings::setTotalFramesToAccumulate( uint32_t totalFramesToAccumulate )
  {
    _totalFramesToAccumulate = totalFramesToAccumulate;
    components::frameCount   = 0;
  }
} // namespace RAYEX_NAMESPACE
