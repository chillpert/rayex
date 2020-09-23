#ifndef UTIL_HPP
#define UTIL_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  namespace util
  {
    /// Parses a given shader file.
    /// @param path The full path to shader file.
    /// @return Returns a vector of chars that contains the shader in SPIR-V format.
    std::vector<char> parseShader( const std::string& path );

    /// Used to find any given element inside a STL container.
    /// @param value The value to search for.
    /// @param values The STL container of the same type as value.
    /// @return Returns true, if value was found in values.
    template <typename T, typename Container>
    bool find( T value, const Container& values )
    {
      for ( const auto& it : values )
      {
        if ( it == value )
          return true;
      }

      return false;
    }

    std::array<float, 4> vec4toArray( const glm::vec4& vec );
  }
}

#endif // UTIL_HPP