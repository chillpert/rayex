#include "window/Time.hpp"
#include "pch/stdafx.hpp"

namespace RX
{
  int frames = 0;
  float prevTime = 0.0f;

  void Time::update()
  {
    m_time = static_cast<float> (SDL_GetTicks()) / 1000.0f;

    float current_time = m_time;

    frames++;

    if (current_time - prevTime >= 1.0f)
    {
      m_frames = frames;
      frames = 0;
      prevTime = current_time;
    }

    m_deltaTime = current_time - m_prevTime;
    m_prevTime = current_time;
  }

  float Time::m_time;
  float Time::m_deltaTime;
}