#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "AccelerationStructure.hpp"
#include "Model.hpp"

namespace vk
{
  namespace Helper
  {
    PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo( ShaderStageFlagBits stage, ShaderModule module, const char* name = "main", SpecializationInfo* specializationInfo = nullptr );

    ImageCreateInfo getImageCreateInfo( Extent3D extent );

    SamplerCreateInfo getSamplerCreateInfo( );

    uint32_t findType( PhysicalDevice physicalDevice, uint32_t typeFilter, MemoryPropertyFlags properties );

    std::vector<char> parseShader( const std::string& path );

    std::vector<ImageView> unpack( const std::vector<vk::UniqueImageView>& uinqueImageViews );

    std::vector<std::shared_ptr<rx::Model>> unpack( const std::unordered_map<std::string, std::shared_ptr<rx::Model>>& models );

    std::vector<DescriptorPoolSize> getPoolSizes( const std::vector<DescriptorSetLayoutBinding>& layoutBindings );
  }
}

#endif // HELPERS_HPP