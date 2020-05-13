#include "api/Shader.hpp"

namespace RX
{
  void Shader::create(const std::string& fullPath, VkDevice device)
  {
    std::string delimiter = "/";

    size_t pos = fullPath.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      pathToFile = fullPath.substr(0, pos + 1).c_str();
      fileName = fullPath.substr(pos + 1).c_str();
    }
    else
      VK_ERROR("Can not process shader paths");

    compile(device);
  }

  void Shader::compile(VkDevice device)
  {
    // This is the name of the resulting shader.
    // For example, myShader.frag will turn into myShader_frag.spv
    fileNameOut = fileName;
    std::string delimiter = ".";

    size_t pos = fileName.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
      std::replace(fileNameOut.begin(), fileNameOut.end(), '.', '_');
      fileNameOut += ".spv";
    }
    else
      VK_ERROR("Can not process shader file name");

    std::stringstream command;
    command << "cd " << pathToFile << " && " << RX_GLSLC_PATH << " " << fileName << " -o " << fileNameOut;
    std::system(command.str().c_str());

    load(device);
  }

  void Shader::destroy(VkDevice device)
  {
    vkDestroyShaderModule(device, shaderModule, nullptr);
  }

  void Shader::load(VkDevice device)
  {
    std::ifstream file(pathToFile + fileNameOut, std::ios::ate | std::ios::binary);

    if (!file.is_open())
      VK_ERROR("Failed to open file");

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    source = buffer;
  
    createShaderModule(device);
  }

  void Shader::createShaderModule(VkDevice device)
  {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

    if (device != VK_NULL_HANDLE)
    {
      VK_ASSERT(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule), "Failed to create shader module");
    }
    else
      VK_ERROR("Failed to create shader module, because a logical device has not been created yet");
  }
}