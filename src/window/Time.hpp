#ifndef TIME_HPP
#define TIME_HPP

#include "Core.hpp"

namespace RX
{
  class Time
  {
  public:
    RX_API static float getTime();
    RX_API static float getDeltaTime();

    void update();

  private:
    static float m_time;
    static float m_deltaTime;

    float m_prevTime;
    float m_frames;
  };
}

#endif // TIME_HPP