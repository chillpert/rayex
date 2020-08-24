#include "Pipeline.hpp"
#include "Vertex.hpp"
#include "Helpers.hpp"
#include "Components.hpp"
#include "RayTracingBuilder.hpp"

namespace rx
{
  Pipeline::Pipeline( vk::Viewport viewport,
                                          vk::Rect2D scissor,
                                          const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                                          vk::ShaderModule rayGen,
                                          vk::ShaderModule miss,
                                          vk::ShaderModule closestHit,
                                          uint32_t maxRecursion,
                                          bool initialize )
  {
    if ( initialize )
      init( viewport, scissor, descriptorSetLayouts, rayGen, miss, closestHit, maxRecursion );
  }

  void Pipeline::init( vk::Viewport viewport,
                                 vk::Rect2D scissor,
                                 const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                                 vk::ShaderModule rayGen,
                                 vk::ShaderModule miss,
                                 vk::ShaderModule closestHit,
                                 uint32_t maxRecursion )
  {
    vk::PushConstantRange pushConstantRange( vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eClosestHitKHR | vk::ShaderStageFlagBits::eMissKHR, // stageFlags
                                             0,                                                                                                                 // offset
                                             sizeof( RayTracingBuilder::PushConstant ) );                                                                       // size

    vk::PipelineLayoutCreateInfo layoutInfo( { },                                                   // flags
                                             static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                             descriptorSetLayouts.data( ),                          // pSetLayouts
                                             1,                                                     // pushConstantRangeCount
                                             &pushConstantRange );                                  // pPushConstantRanges

    m_layout = g_device.createPipelineLayoutUnique( layoutInfo );
    if ( !m_layout )
      RX_ERROR( "Failed to create pipeline layout." );

    std::array<vk::PipelineShaderStageCreateInfo, 3> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eRaygenKHR, rayGen );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, miss );
    shaderStages[2] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eClosestHitKHR, closestHit );

    // Set up raytracing shader groups.
    std::array<vk::RayTracingShaderGroupCreateInfoKHR, 3> groups;

    for ( auto& group : groups )
    {
      group.generalShader = VK_SHADER_UNUSED_KHR;
      group.closestHitShader = VK_SHADER_UNUSED_KHR;
      group.anyHitShader = VK_SHADER_UNUSED_KHR;
      group.intersectionShader = VK_SHADER_UNUSED_KHR;
    }

    groups[0].generalShader = RX_SHADER_GROUP_INDEX_RGEN;
    groups[0].type = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[1].generalShader = RX_SHADER_GROUP_INDEX_MISS;
    groups[1].type = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[2].closestHitShader = RX_SHADER_GROUP_INDEX_CHIT;
    groups[2].type = vk::RayTracingShaderGroupTypeKHR::eTrianglesHitGroup;

    g_shaderGroups = static_cast<uint32_t>( groups.size( ) );

    vk::RayTracingPipelineCreateInfoKHR createInfo( { },                                           // flags
                                                    static_cast<uint32_t>( shaderStages.size( ) ), // stageCount
                                                    shaderStages.data( ),                          // pStages
                                                    static_cast<uint32_t>( groups.size( ) ),       // groupCount
                                                    groups.data( ),                                // pGroups
                                                    maxRecursion,                                  // maxRecursionDepth
                                                    0,                                             // libraries
                                                    nullptr,                                       // pLibraryInterface
                                                    m_layout.get( ),                               // layout
                                                    nullptr,                                       // basePipelineHandle
                                                    0 );                                           // basePipelineIndex
  
    m_pipeline = g_device.createRayTracingPipelineKHRUnique( nullptr, createInfo );
  }

  void Pipeline::bind( vk::CommandBuffer commandBuffer ) const
  {
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eRayTracingKHR, m_pipeline.get( ) );
  }
}