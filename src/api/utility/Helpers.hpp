#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "stdafx.hpp"

namespace vk
{
  namespace Helper
  {
    PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo( ShaderStageFlagBits stage, ShaderModule module, const char* name = "main", SpecializationInfo* specializationInfo = nullptr );

    ImageCreateInfo getImageCreateInfo( Extent3D extent );

    SamplerCreateInfo getSamplerCreateInfo( );

    uint32_t findType( PhysicalDevice physicalDevice, uint32_t typeFilter, MemoryPropertyFlags properties );

    std::vector<char> parseShader( const std::string& path );

    std::vector<vk::ImageView> unpack( const std::vector<vk::UniqueImageView>& uinqueImageViews );
  }
}

#endif // HELPERS_HPP