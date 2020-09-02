#ifndef TIME_HPP
#define TIME_HPP

#include "Core.hpp"
#include <vector>

namespace rx
{
  /// Used to keep track of the application's timing.
  class Time
  {
  public:
    /// Destructor of Time.
    /// 
    /// Prints a message containing average FPS.
    /// @todo Average FPS are pointless. Implement minimum FPS and frametimes instead.
    RX_API ~Time( );

    /// @return Returns the time passed since application start in seconds.
    RX_API static float getTime( );
    /// @return Returns the time passed between the current and the last frame.
    RX_API static float getDeltaTime( );

    /// Updates the timing.
    /// 
    /// Prints the current FPS every three seconds.
    /// @note This function will be called every tick.
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