#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Shader
  {
  public:
    Shader(const char* path);

    void parse();

  private:
    const char* m_path;
  };
}

#endif // SHADER_HPP