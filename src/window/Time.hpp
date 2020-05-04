#ifndef TIME_HPP
#define TIME_HPP

#include "Core.hpp"

namespace RX
{
  class Time
  {
  public:
    static inline float getTime() { return m_time; }
    static inline float getDeltaTime() { return m_deltaTime; }

    void update();

  private:
    static float m_time;
    static float m_deltaTime;

    float m_prevTime;
    float m_frames;
  };
}

#endif // TIME_HPP