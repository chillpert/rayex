#include "base/Time.hpp"
#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  float Time::time;
  float Time::deltaTime;

  int frames = 0;
  float prevTime = 0.0f;

  const float timeToWaitForStartingBenchmark = 3.0f;

  Time::~Time( )
  {
    this->allFrames;

    int res = 0;
    for ( int value : this->allFrames )
      res += value;

    if ( time >= timeToWaitForStartingBenchmark && this->allFrames.size( ) > 0 )
      std::cout << "Average FPS: " << static_cast<float>( res ) / static_cast<float>( this->allFrames.size( ) ) << "\nBenchmark Length: " << time / 60.0f << " minutes." << std::endl;
  }

  float Time::getTime( )
  {
    return time;
  }

  float Time::getDeltaTime( )
  {
    return deltaTime;
  }

  void Time::update( )
  {
    time = static_cast<float> ( SDL_GetTicks( ) ) / 1000.0f;

    float current_time = time;

    frames++;

    if ( current_time - prevTime >= 1.0f )
    {
      // Give the application some time to start before recording the fps.
      if ( time > timeToWaitForStartingBenchmark )
      {
        std::cout << "FPS: " << frames << std::endl;
        this->allFrames.push_back( frames );
      }

      this->frames = frames;
      frames = 0;
      prevTime = current_time;
    }

    deltaTime = current_time - this->prevTime;
    this->prevTime = current_time;

    //std::cout << "Frametime: " << deltaTime << std::endl;
  }
}