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

    /*
    template <typename Out, typename In>
    std::vector<Out> unpack( const std::vector<In>& data )
    {
      std::vector<Out> result( data.size( ) );

      for ( size_t i = 0; i < result.size( ); ++i )
        result[i] = data[i].get( );

      return result;
    }
    */

    std::vector<ImageView> unpack( const std::vector<UniqueImageView>& uniqueImageViews );

    std::vector<Framebuffer> unpack( const std::vector<UniqueFramebuffer>& uniqueFramebuffers );

    std::vector<std::shared_ptr<rx::Model>> unpack( const std::unordered_map<std::string, std::shared_ptr<rx::Model>>& models );

    std::vector<DescriptorPoolSize> getPoolSizes( const std::vector<DescriptorSetLayoutBinding>& layoutBindings );

    void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout );
    void transitionImageLayout( Image image, ImageLayout oldLayout, ImageLayout newLayout, vk::CommandBuffer commandBuffer );

    std::tuple<vk::ImageMemoryBarrier, vk::PipelineStageFlags, vk::PipelineStageFlags> getImageMemoryBarrierInfo( vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout );
  }
}

#endif // HELPERS_HPP