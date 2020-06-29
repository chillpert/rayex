#ifndef INITIALIZERS_HPP
#define INITIALIZERS_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  namespace Initializers
  {
    vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo(vk::ShaderStageFlagBits stage, vk::ShaderModule module, const char* name = "main", vk::SpecializationInfo* specializationInfo = nullptr);
    
    vk::ImageCreateInfo getImageCreateInfo(vk::Extent3D extent);
  }
}

#endif // INITIALIZERS_HPP