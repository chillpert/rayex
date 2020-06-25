#include "Shader.hpp"

namespace RX
{
  Shader::Shader(ShaderInfo& info)
  {
    initialize(info);
  }

  Shader::~Shader()
  {
    if (m_shaderModule)
      destroy();
  }

  void Shader::initialize(ShaderInfo& info)
  {
    m_info = info;

    std::string delimiter = "/";

    size_t pos = m_info.fullPath.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      m_info.pathToFile = m_info.fullPath.substr(0, pos + 1).c_str();
      m_info.fileName = m_info.fullPath.substr(pos + 1).c_str();
    }
    else
      RX_ERROR("Can not process shader paths.");

    // This is the name of the resulting shader.
    // For example, myShader.frag will turn into myShader_frag.spv.
    m_info.fileNameOut = m_info.fileName;
    delimiter = ".";

    pos = m_info.fileName.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      std::replace(m_info.fileNameOut.begin(), m_info.fileNameOut.end(), '.', '_');
      m_info.fileNameOut += ".spv";
    }
    else
      RX_ERROR("Can not process shader file name.");

    // Calls glslc to compile the glsl file into spir-v.
    std::stringstream command;
    command << "cd " << m_info.pathToFile << " && " << RX_GLSLC_PATH << " " << m_info.fileName << " -o " << m_info.fileNameOut;
    std::system(command.str().c_str());

    // Read the file and retrieve the source.
    std::ifstream file(m_info.pathToFile + m_info.fileNameOut, std::ios::ate | std::ios::binary);

    if (!file.is_open())
      RX_ERROR("Failed to open shader source file.");

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    m_info.source = buffer;

    // Create the shader module.
    vk::ShaderModuleCreateInfo createInfo;
    createInfo.codeSize = m_info.source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(m_info.source.data());

    m_shaderModule = m_info.device.createShaderModule(createInfo);
    if (!m_shaderModule)
      RX_ERROR("Failed to create shader module.");
  }

  void Shader::destroy()
  {
    m_info.device.destroyShaderModule(m_shaderModule);
    m_shaderModule = nullptr;
  }
}