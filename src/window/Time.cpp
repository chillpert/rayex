#include "window/Time.hpp"
#include "pch/stdafx.hpp"

namespace RX
{
  float Time::m_time;
  float Time::m_deltaTime;

  int frames = 0;
  float prevTime = 0.0f;

  Time::~Time()
  {
    m_allFrames;

    int res = 0;
    for (int value : m_allFrames)
      res += value;

    RX_LOG("Average FPS: " << static_cast<float>(res) / static_cast<float>(m_allFrames.size()) << " over time: " << m_time / 60.0f << " minutes.");
  }

  float Time::getTime()
  {
    return m_time;
  }

  float Time::getDeltaTime()
  {
    return m_deltaTime;
  }

  void Time::update()
  {
    m_time = static_cast<float> (SDL_GetTicks()) / 1000.0f;

    float current_time = m_time;

    frames++;

    if (current_time - prevTime >= 1.0f)
    {
      std::cout << "FPS: " << frames << std::endl;

      // Give the application some time to start before recording the fps.
      if (m_time > 3.0f)
        m_allFrames.push_back(frames);

      m_frames = frames;
      frames = 0;
      prevTime = current_time;
    }

    m_deltaTime = current_time - m_prevTime;
    m_prevTime = current_time;
  }
}