#include "api/Shader.hpp"

namespace RX
{
  Shader::Shader() :
    BaseComponent("Shader") { }

  Shader::~Shader()
  {
    destroy();
  }

  void Shader::initialize(const std::string& fullPath, VkDevice device)
  {
    m_device = device;

    std::string delimiter = "/";

    size_t pos = fullPath.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      m_pathToFile = fullPath.substr(0, pos + 1).c_str();
      m_fileName = fullPath.substr(pos + 1).c_str();
    }
    else
      RX_ERROR("Can not process shader paths.");

    compile(device);
  }

  void Shader::destroy()
  {
    assertDestruction();
    vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
  }

  void Shader::compile(VkDevice device)
  {
    // This is the name of the resulting shader.
    // For example, myShader.frag will turn into myShader_frag.spv.
    m_fileNameOut = m_fileName;
    std::string delimiter = ".";

    size_t pos = m_fileName.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      std::replace(m_fileNameOut.begin(), m_fileNameOut.end(), '.', '_');
      m_fileNameOut += ".spv";
    }
    else
      RX_ERROR("Can not process shader file name.");

    // Calls glslc to compile the glsl file into spir-v.
    std::stringstream command;
    command << "cd " << m_pathToFile << " && " << RX_GLSLC_PATH << " " << m_fileName << " -o " << m_fileNameOut;
    std::system(command.str().c_str());

    load(device);
  }

  void Shader::load(VkDevice device)
  {
    std::ifstream file(m_pathToFile + m_fileNameOut, std::ios::ate | std::ios::binary);

    if (!file.is_open())
      RX_ERROR("Failed to open shader source file.");

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    m_source = buffer;
  
    createShaderModule(device);
  }

  void Shader::createShaderModule(VkDevice device)
  {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = m_source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(m_source.data());

    if (device != VK_NULL_HANDLE)
    {
      VK_ASSERT(vkCreateShaderModule(device, &createInfo, nullptr, &m_shaderModule), "Failed to create shader module.");
    }
    else
      RX_ERROR("Failed to create shader module, because a logical device has not been created yet.");
  
    initializationCallback();
  }
}