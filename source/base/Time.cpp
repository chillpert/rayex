#include "base/Time.hpp"
#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  float Time::m_time;
  float Time::m_deltaTime;

  int frames = 0;
  float prevTime = 0.0f;

  const float timeToWaitForStartingBenchmark = 3.0f;

  Time::~Time( )
  {
    m_allFrames;

    int res = 0;
    for ( int value : m_allFrames )
      res += value;

    if ( m_time >= timeToWaitForStartingBenchmark && m_allFrames.size( ) > 0 )
      std::cout << "Average FPS: " << static_cast<float>( res ) / static_cast<float>( m_allFrames.size( ) ) << "\nBenchmark Length: " << m_time / 60.0f << " minutes." << std::endl;
  }

  float Time::getTime( )
  {
    return m_time;
  }

  float Time::getDeltaTime( )
  {
    return m_deltaTime;
  }

  void Time::update( )
  {
    m_time = static_cast<float> ( SDL_GetTicks( ) ) / 1000.0f;

    float current_time = m_time;

    frames++;

    if ( current_time - prevTime >= 1.0f )
    {
      // Give the application some time to start before recording the fps.
      if ( m_time > timeToWaitForStartingBenchmark )
      {
        std::cout << "FPS: " << frames << std::endl;
        m_allFrames.push_back( frames );
      }

      m_frames = frames;
      frames = 0;
      prevTime = current_time;
    }

    m_deltaTime = current_time - m_prevTime;
    m_prevTime = current_time;

    //std::cout << "Frametime: " << m_deltaTime << std::endl;
  }
}