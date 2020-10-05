#include "base/Time.hpp"

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  float Time::s_time;
  float Time::s_deltaTime;

  int frames      = 0;
  float prevTime2 = 0.0F;

  const float timeToWaitForStartingBenchmark = 3.0F;

  auto Time::getTime( ) -> float
  {
    return s_time;
  }

  auto Time::getDeltaTime( ) -> float
  {
    return s_deltaTime;
  }

  void Time::update( )
  {
    s_time = static_cast<float>( SDL_GetTicks( ) ) / 1000.0F;

    float current_time = s_time;

    frames++;

    rx::Time::s_deltaTime = ( current_time - this->prevTime );
    this->prevTime        = current_time;

    // Print fps every full second.
    if ( current_time - prevTime2 >= 1.0F )
    {
      // Give the application some time to start before recording the fps.
      if ( s_time > timeToWaitForStartingBenchmark )
      {
        RX_VERBOSE( "FPS: ", frames );
        this->allFrames.push_back( frames );
      }

      this->frames = frames;
      frames       = 0;
      prevTime2    = current_time;
    }
  }
} // namespace RAYEXEC_NAMESPACE