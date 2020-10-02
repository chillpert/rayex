#include "base/Time.hpp"

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  float Time::s_time;
  float Time::s_deltaTime;

  int frames      = 0;
  float prevTime2 = 0.0f;

  const float timeToWaitForStartingBenchmark = 3.0f;

  Time::~Time( )
  {
    this->allFrames;

    int res = 0;
    for ( int value : this->allFrames )
      res += value;

    if ( s_time >= timeToWaitForStartingBenchmark && this->allFrames.size( ) > 0 )
      std::cout << "Average FPS: " << static_cast<float>( res ) / static_cast<float>( this->allFrames.size( ) ) << "\nBenchmark Length: " << s_time / 60.0f << " minutes." << std::endl;
  }

  float Time::getTime( )
  {
    return s_time;
  }

  float Time::getDeltaTime( )
  {
    return s_deltaTime;
  }

  void Time::update( )
  {
    s_time = static_cast<float>( SDL_GetTicks( ) ) / 1000.0f;

    float current_time = s_time;

    frames++;

    this->s_deltaTime = ( current_time - this->prevTime );
    this->prevTime    = current_time;

    // Print fps every full second.
    if ( current_time - prevTime2 >= 1.0f )
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