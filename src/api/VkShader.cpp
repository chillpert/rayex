#include "api/VkShader.hpp"

namespace RX
{
  VkShader::VkShader(const std::string& pathToFile, const std::string& fileName, VkDevice* device) :
    m_pathToFile(pathToFile), m_fileName(fileName), m_logicalDevice(device)
  {
    compile();
  }

  VkShader::VkShader(const std::string& fullPath, VkDevice* device)
    : m_logicalDevice(device)
  {
    std::string delimiter = "/";

    size_t pos = fullPath.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      m_pathToFile = fullPath.substr(0, pos + 1).c_str();
      m_fileName = fullPath.substr(pos + 1).c_str();
    }
    else
    {
      Error::runtime("Can not process shader paths", Error::SHADER);
    }

    compile();
  }

  void VkShader::compile()
  {
    // This is the name of the resulting shader.
    // For example, myShader.frag will turn into myShader_frag.spv
    m_fileNameOut = m_fileName;
    std::string delimiter = ".";

    size_t pos = m_fileName.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      std::replace(m_fileNameOut.begin(), m_fileNameOut.end(), '.', '_');
      m_fileNameOut += ".spv";
    }
    else
    {
      Error::runtime("Can not process shader file name", Error::SHADER);
    }

#ifdef RX_PLATFORM_WINDOWS_X64
    std::stringstream command;
    command << "cd " << m_pathToFile << " && " << RX_GLSLC_PATH << " " << m_fileName << " -o " << m_fileNameOut;

    std::system(command.str().c_str());
#elif RX_PLATFORM_LINUX_X64
#error TODO: Linux shader compilation
#endif

    load();
  }

  void VkShader::destroy()
  {
    vkDestroyShaderModule(*m_logicalDevice, m_shaderModule, nullptr);
  }

  void VkShader::load()
  {
    std::ifstream file(m_pathToFile + m_fileNameOut, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
      Error::runtime("Failed to open file", Error::SHADER);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    m_source = buffer;
  
    createShaderModule();
  }

  void VkShader::createShaderModule()
  {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = m_source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(m_source.data());

    if (m_logicalDevice != nullptr)
    {
      Assert::vulkan(vkCreateShaderModule(*m_logicalDevice, &createInfo, nullptr, &m_shaderModule), "Failed to create shader module");
    }
    else
    {
      Error::runtime("Failed to create shader module, because a logical device has not been created yet", Error::SHADER);
    }


  }
}