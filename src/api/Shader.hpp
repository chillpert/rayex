#ifndef SHADER_HPP
#define SHADER_HPP

#include "BaseComponent.hpp"

namespace RX
{
  struct ShaderInfo
  {
    std::string fullPath;
    VkDevice device;

    std::string pathToFile; // Ignore, will be initialized automatically.
    std::string fileName; // Ignore, will be initialized automatically.
    std::string fileNameOut; // Ignore, will be initialized automatically.
    std::vector<char> source; // Ignore, will be initialized automatically.
  };

  class Shader
  {
  public:
    ~Shader();

    inline VkShaderModule get() { return m_shaderModule; }
    inline ShaderInfo getInfo() { return m_info; }

    void initialize(ShaderInfo& info);
    void destroy();

  private:
    void createShaderModule();

    VkShaderModule m_shaderModule;
    ShaderInfo m_info;
  };
}

#endif // SHADER_HPP