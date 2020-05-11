#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Shader
  {
  public:
    void create(const std::string& fullPath, VkDevice device);
    void destroy(VkDevice device);

    inline std::vector<char>& getSource() { return source; }
    inline VkShaderModule getShaderModule() { return shaderModule; }

  private:
    // This function calls glslc.exe to compile the given shader file to .spv.
    void compile(VkDevice device);
    // This function opens the .spv file and retrieves the source code
    void load(VkDevice device);
    void createShaderModule(VkDevice device);

    std::string pathToFile;

    std::string fileName;
    std::string fileNameOut;

    std::vector<char> source;

    VkShaderModule shaderModule;
  };
}

#endif // SHADER_HPP