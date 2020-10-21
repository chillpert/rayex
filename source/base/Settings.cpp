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

  void Settings::setEnableRayTracing( bool flag )
  {
    _rayTrace         = flag;
    _refreshSwapchain = true;

    components::frameCount = 0;
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
    _maxGeometryInstances = amount;
  }

  void Settings::setDefaultAssetsPath( )
  {
    _assetsPath = std::filesystem::current_path( ).string( ) += "/";

    std::replace( _assetsPath.begin( ), _assetsPath.end( ), '\\', '/' );

    components::assetsPath = _assetsPath;
    RX_WARN( "No path to assets specified. Using default path as path to resources: ", _assetsPath );
  }

  void Settings::setEnableJitterCam( bool flag )
  {
    _jitterCamEnabled      = flag;
    _ssaaEnabled           = false;
    components::frameCount = 0;
  }

  void Settings::setJitterCamSampleRate( uint32_t sampleRate )
  {
    // Make sure the sample rate is always greater than zero.
    _jitterCamSampleRate   = sampleRate > 0 ? sampleRate : 1;
    components::frameCount = 0;
  }

  void Settings::setJitterCamSampleRatePerRayGen( uint32_t sampleRate )
  {
    // Make sure the sample rate per raygen is always greater than zero.
    _jitterCamSampleRatePerRayGen = sampleRate > 0 ? sampleRate : 1;
    components::frameCount        = 0;
  }

  void Settings::setEnableSsaa( bool flag )
  {
    _ssaaEnabled      = flag;
    _jitterCamEnabled = false;
  }

  void Settings::setSsaaSampleRate( uint32_t sampleRate )
  {
    _ssaaSampleRate = sampleRate > 0 ? sampleRate : 1;
  }
} // namespace RAYEX_NAMESPACE
