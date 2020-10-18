#include "base/Time.hpp"

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  float deltaTime;
  float prevTime;
  std::vector<uint32_t> allFrames;

  uint32_t frames = 0;
  float prevTime2 = 0.0F;

  const float timeToWaitForStartingBenchmark = 3.0F;

  void Time::benchmark( )
  {
    if ( allFrames.size( ) == 0 )
    {
      return;
    }

    uint32_t sum = 0.0F;
    for ( int value : allFrames )
    {
      sum += value;
    }

    float average = static_cast<float>( sum ) / static_cast<float>( allFrames.size( ) );

    RX_INFO( "Average fps benchmark result: ", average, " ( Time: ", getTime( ) - timeToWaitForStartingBenchmark, " s )." );
  }

  auto Time::getTime( ) -> float
  {
    return static_cast<float>( SDL_GetTicks( ) ) / 1000.0F;
  }

  auto Time::getDeltaTime( ) -> float
  {
    return deltaTime;
  }

  void Time::update( )
  {
    float current_time = static_cast<float>( SDL_GetTicks( ) ) / 1000.0F;

    frames++;

    deltaTime = ( current_time - prevTime );
    prevTime  = current_time;

    // Print fps every full second.
    if ( current_time - prevTime2 >= 1.0F )
    {
      // Give the application some time to start before recording the fps.
      if ( current_time > timeToWaitForStartingBenchmark )
      {
        RX_VERBOSE( "FPS: ", frames );
        allFrames.push_back( frames );
      }

      frames    = frames;
      frames    = 0;
      prevTime2 = current_time;
    }
  }
} // namespace RAYEXEC_NAMESPACE
