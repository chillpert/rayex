#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Shader
  {
  public:
    Shader(const std::string& pathToFile, const std::string& fileName, VkDevice* device = nullptr);
    Shader(const std::string& fullPath, VkDevice* device = nullptr);

    // This function calls glslc.exe to compile the given shader file to .spv.
    void compile();
    void destroy();

    inline std::vector<char>& getSource() { return m_source; }
    inline VkShaderModule* getShaderModule() { return &m_shaderModule; }

  private:
    // This function opens the .spv file and retrieves the source code
    void load();
    void createShaderModule();

    std::string m_pathToFile;

    std::string m_fileName;
    std::string m_fileNameOut;

    std::vector<char> m_source;

    VkShaderModule m_shaderModule;
    VkDevice* m_logicalDevice;
  };
}

#endif // SHADER_HPP