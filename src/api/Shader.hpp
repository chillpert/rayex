#ifndef SHADER_HPP
#define SHADER_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class Shader : public BaseComponent
  {
  public:
    Shader();
    ~Shader();

    void initialize(const std::string& fullPath, VkDevice device);
    void destroy();

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
    VkDevice m_device;
  };
}

#endif // SHADER_HPP