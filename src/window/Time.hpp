#ifndef TIME_HPP
#define TIME_HPP

#include "Core.hpp"
#include <vector>

namespace rx
{
  class Time
  {
  public:
    RX_API ~Time( );

    RX_API static float getTime( );
    RX_API static float getDeltaTime( );

    void update( );

  private:
    static float m_time;
    static float m_deltaTime;

    float m_prevTime;
    float m_frames;

    std::vector<int> m_allFrames;
    std::vector<float> m_allFrametimes;
  };
}

#endif // TIME_HPP