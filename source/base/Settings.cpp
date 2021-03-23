#include "base/Settings.hpp"

#include "api/Components.hpp"

namespace RAYEX_NAMESPACE
{
  void Settings::setPathDepth( uint32_t recursionDepth )
  {
    if ( recursionDepth <= _maxPathDepth )
    {
      _pathDepth = recursionDepth;
    }
    else
    {
      _pathDepth = _maxPathDepth;
      RX_WARN( "Exceeded maximum path depth of ", _maxPathDepth, ". Using highest possible value instead." );
    }

    _pathDepth = recursionDepth;
    //triggerPipelineRefresh( );
  }

  void Settings::setClearColor( const glm::vec4& clearColor )
  {
    static bool firstRun = true;

    if ( firstRun )
    {
      firstRun = false;
    }
    else
    {
      triggerSwapchainRefresh( );
    }

    _clearColor = clearColor;

    components::frameCount = -1;
  }

  void Settings::setNextEventEstimation( bool flag )
  {
    _nextEventEstimation = flag;
  }

  void Settings::setNextEventEstimationMinBounces( uint32_t minBounces )
  {
    _nextEventEstimationMinBounces = minBounces;
  }

  void Settings::setRussianRoulette( bool flag )
  {
    _russianRoulette = flag;
  }

  void Settings::setRussianRouletteMinBounces( uint32_t minBounces )
  {
    _russianRouletteMinBounces = minBounces;
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

  void Settings::setGeometryInstanceLimit( uint32_t amount )
  {
    if ( amount == 0 )
    {
      ++amount;
      RX_WARN( "Can not use value 0 for the maximum amount of geometry instances. Using 1 instead." );
    }

    // Increment by one to accommodate the triangle dummy for emtpy scenes.
    _maxGeometryInstances = amount;

    _maxGeometryInstancesChanged = true;
  }

  void Settings::setGeometryLimit( size_t amount )
  {
    if ( amount == 0 )
    {
      RX_WARN( "Can not use value 0 for the maximum number of geometries. Using 16 instead." );
      amount = 16;
    }

    if ( amount < 16 )
    {
      RX_WARN( "Can not use value ", amount, " for the maximum number of geometries. Using 16 instead." );
      amount = 16;
    }

    if ( amount % 4 != 0 )
    {
      RX_WARN( "Minimum storage buffer for geometries alignment must be a multiple of 4. Using 16 instead." );
      amount = 16;
    }

    _maxGeometry = ++amount;

    _maxGeometryChanged = true;
  }

  void Settings::setTextureLimit( size_t amount )
  {
    if ( amount == 0 )
    {
      amount;
      RX_WARN( "Can not use value 0 for the maximum amount of textures. Using 1 instead." );
    }

    _maxTextures = ++amount;

    _maxTexturesChanged = true;
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

  void Settings::setAccumulatingFrames( bool flag )
  {
    _accumulateFrames = flag;
  }
} // namespace RAYEX_NAMESPACE
