#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "stdafx.hpp"

namespace rx
{
  /// Exposes all graphic settings supported by the renderer.
  class RX_API Settings
  {
  public:
    /// Used to set the maximum recursion depth of the path tracer.
    /// If this function is called, it will force the renderer to recreate the graphics pipeline the next time it will be used.
    /// @param maxRecursionDepth The maximum recursion depth.
    static void setMaxRecursionDepth( uint32_t maxRecursionDepth );

    /// @return Returns the maximum recursion depth.
    inline static uint32_t getMaxRecursionDepth( ) { return s_maxRecursionDepth; }

    /// @return Returns true if the graphics pipeline needs to be re-created.
    static bool refresh( ) { return s_refresh; }

  private:
    static uint32_t s_maxRecursionDepth; ///< The maximum recursion depth.
    static bool s_refresh; ///< Keeps track of whether or not the graphics pipeline needs to be re-created.
  };
}

#endif // SETTINGS_HPP