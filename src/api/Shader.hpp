#ifndef SHADER_HPP
#define SHADER_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class Shader : public BaseComponent
  {
  public:
    Shader();

    void create(const std::string& fullPath, VkDevice device);
    void destroy(VkDevice device);

    inline std::vector<char>& getSource() { return m_source; }
    inline VkShaderModule getShaderModule() { return m_shaderModule; }

  private:
    // This function calls glslc.exe to compile the given shader file to .spv.
    void compile(VkDevice device);
    // This function opens the .spv file and retrieves the source code
    void load(VkDevice device);
    void createShaderModule(VkDevice device);

    std::string m_pathToFile;
    std::string m_fileName;
    std::string m_fileNameOut;

    std::vector<char> m_source;

    VkShaderModule m_shaderModule;
  };
}

#endif // SHADER_HPP