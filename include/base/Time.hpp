#ifndef TIME_HPP
#define TIME_HPP

#include "Core.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Used to keep track of the application's timing.
  /// @todo Average FPS are pointless. Implement minimum FPS and frametimes instead.
  class Time
  {
  public:
    /// Destructor of Time.
    ///
    /// Prints a message containing average FPS.
    RX_API ~Time( );

    /// @return Returns the time passed since application start in seconds.
    RX_API static auto getTime( ) -> float;
    /// @return Returns the time passed between the current and the last frame.
    RX_API static auto getDeltaTime( ) -> float;

    /// Updates the timing.
    ///
    /// Prints the current FPS every three seconds.
    /// @note This function will be called every tick.
    void update( );

  private:
    static float s_time;      ///< The time passed since application start in seconds.
    static float s_deltaTime; ///< The time passed between the current and the last frame.

    float prevTime; ///< Stores the prev time in intervals of one second.
    size_t frames;  ///< The frame counter; Gets reset after each second.

    std::vector<int> allFrames; ///< Stores all frame rate values measured in intervals of one second.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // TIME_HPP