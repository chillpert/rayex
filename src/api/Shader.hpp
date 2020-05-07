#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Shader
  {
  public:
    Shader(const std::string& pathToFile, const std::string& fileName);
    Shader(const std::string& fullPath);

    // This function calls glslc.exe to compile the given shader file to .spv.
    void compile();

    inline std::vector<char>& getSource() { return m_source; }

  private:
    // This function opens the .spv file and retrieves the source code
    void load();

    std::string m_pathToFile;

    std::string m_fileName;
    std::string m_fileNameOut;

    std::vector<char> m_source;
  };
}

#endif // SHADER_HPP