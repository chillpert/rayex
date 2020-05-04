#ifndef TIMER_HPP
#define TIMER_HPP

#include "Core.hpp"

namespace RX
{
  class Timer
  {
  public:
    RX_API inline float getTime() const { return m_time; }
    RX_API inline float getDeltaTime() const { return m_deltaTime; }

  private:
    float m_time;
    float m_deltaTime;
  };
}

#endif // TIMER_HPP