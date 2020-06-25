#ifndef INITIALIZERS_HPP
#define INITIALIZERS_HPP

namespace RX
{
  namespace Initializers
  {
    vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo(vk::ShaderStageFlagBits stage, vk::ShaderModule module, const char* name = "main", vk::SpecializationInfo* specializationInfo = nullptr)
    {
      return vk::PipelineShaderStageCreateInfo
      {
        { },                                  // flags
        vk::ShaderStageFlagBits::eRaygenKHR,  // stage
        module,                               // module
        "main",                               // pName
        specializationInfo                    // pSpecializationInfo
      };
    }
  }
}

#endif // INITIALIZERS_HPP