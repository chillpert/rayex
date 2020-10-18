#ifndef TIME_HPP
#define TIME_HPP

#include "Core.hpp"

namespace RAYEXEC_NAMESPACE
{
  class Window;

  /// Used to keep track of the application's timing.
  /// @todo Average FPS are pointless. Implement minimum FPS and frametimes instead.
  class Time
  {
  public:
    friend Window;

    /// Prints the average fps as well as the time since recording.
    RX_API static void benchmark( );

    /// @return Returns the time passed since application start in seconds.
    RX_API static auto getTime( ) -> float;
    /// @return Returns the time passed between the current and the last frame.
    RX_API static auto getDeltaTime( ) -> float;

  private:
    /// Updates the timing.
    ///
    /// Prints the current fps every three seconds.
    /// @note This function will be called every tick.
    static void update( );
  };
} // namespace RAYEXEC_NAMESPACE

#endif // TIME_HPP
