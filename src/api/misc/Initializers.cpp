#include "Initializers.hpp"

namespace RX
{
  namespace Initializers
  {
    vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo(vk::ShaderStageFlagBits stage, vk::ShaderModule module, const char* name, vk::SpecializationInfo* specializationInfo)
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

    vk::ImageCreateInfo getImageCreateInfo(vk::Extent3D extent)
    {
      vk::ImageCreateInfo createInfo( { },                                                                      // flags
                                      vk::ImageType::e2D,                                                       // imageType
                                      vk::Format::eR8G8B8A8Srgb,                                                // format
                                      extent,                                                                   // extent
                                      1,                                                                        // mipLevels
                                      1,                                                                        // arrayLayers
                                      vk::SampleCountFlagBits::e1,                                              // samples
                                      vk::ImageTiling::eOptimal,                                                // tiling
                                      vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,  // usage
                                      vk::SharingMode::eExclusive,                                              // sharingMode
                                      0,                                                                        // queueFamilyIndexCount
                                      nullptr,                                                                  // pQueueFamilyIndices
                                      vk::ImageLayout::eUndefined );                                            // initialLayout

      return createInfo;
    }
  }
}