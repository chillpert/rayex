#include "Settings.hpp"

#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
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

    g_frameCount = 0;
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

    g_frameCount = 0;
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

  void Settings::setDefaultAssetsPath( )
  {
    this->assetsPath = std::filesystem::current_path( ).string( ) += "/";

    std::replace( this->assetsPath.begin( ), this->assetsPath.end( ), '\\', '/' );

    g_assetsPath = this->assetsPath;
    RX_INFO( "No path to assets specified. Using default path as path to resources: ", this->assetsPath );
  }

  void Settings::setEnableJitterCam( bool flag )
  {
    this->jitterCamEnabled = flag;
    this->msaaEnabled      = false;
    g_frameCount           = 0;
  }

  void Settings::setJitterCamSampleRate( uint32_t sampleRate )
  {
    // Make sure the sample rate is always greater than zero.
    this->jitterCamSampleRate = sampleRate > 0 ? sampleRate : 1;
    g_frameCount              = 0;
  }

  void Settings::setJitterCamSampleRatePerRayGen( uint32_t sampleRate )
  {
    // Make sure the sample rate per raygen is always greater than zero.
    this->jitterCamSampleRatePerRayGen = sampleRate > 0 ? sampleRate : 1;
    g_frameCount                       = 0;
  }

  void Settings::setEnableMsaa( bool flag )
  {
    this->msaaEnabled      = flag;
    this->jitterCamEnabled = false;
  }

  void Settings::setMsaaSampleRate( uint32_t sampleRate )
  {
    if ( sampleRate < 3 )
    {
      this->msaaSampleRate = 4;
      return;
    }

    double t1 = std::log2( sampleRate );
    double t2 = std::fmod( t1, 2.0 );
    double t3 = std::fmod( t2, 1.0 );

    if ( t3 == 0.0 )
    {
      this->msaaSampleRate = sampleRate;
    }
    else
    {
      if ( t3 >= 0.5 )
      {
        this->msaaSampleRate = static_cast<uint32_t>( std::pow( 2.0, std::ceil( t1 ) ) );
      }
      else
      {
        this->msaaSampleRate = static_cast<uint32_t>( std::pow( 2.0, std::floor( t1 ) ) );
      }
    }
  }
} // namespace RAYEXEC_NAMESPACE
