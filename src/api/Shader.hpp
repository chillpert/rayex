#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct ShaderInfo
  {
    std::string fullPath;
    vk::Device device;

    std::string pathToFile; // Ignore, will be initialized automatically.
    std::string fileName; // Ignore, will be initialized automatically.
    std::string fileNameOut; // Ignore, will be initialized automatically.
    std::vector<char> source; // Ignore, will be initialized automatically.
  };

  class Shader
  {
  public:
    Shader() = default;
    Shader(ShaderInfo& info);
    Shader(ShaderInfo&& info);
    ~Shader();

    inline vk::ShaderModule get() const { return m_shaderModule; }
    
    void init(ShaderInfo& info);

    void destroy();

  private:
    vk::ShaderModule m_shaderModule;
    ShaderInfo m_info;
  };
}

#endif // SHADER_HPP