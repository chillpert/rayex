#include "Pipeline.hpp"
#include "Vertex.hpp"
#include "Helpers.hpp"
#include "Components.hpp"
#include "RayTracingBuilder.hpp"

namespace rx
{
  RasterizationPipeline::RasterizationPipeline( vk::RenderPass renderPass,
                                                vk::Viewport viewport,
                                                vk::Rect2D scissor,
                                                vk::ShaderModule vertexShader,
                                                vk::ShaderModule fragmentShader,
                                                vk::DescriptorSetLayout descriptorSetLayout,
                                                vk::PrimitiveTopology topology,
                                                bool initialize )
  {
    if ( initialize )
      init( renderPass, viewport, scissor, vertexShader, fragmentShader, descriptorSetLayout, topology );
  }

  void RasterizationPipeline::init( vk::RenderPass renderPass,
                                    vk::Viewport viewport,
                                    vk::Rect2D scissor,
                                    vk::ShaderModule vertexShader,
                                    vk::ShaderModule fragmentShader,
                                    vk::DescriptorSetLayout descriptorSetLayout,
                                    vk::PrimitiveTopology topology )
  {
    m_scissor = scissor;
    m_viewport = viewport;

    // TODO: this has to be more adjustable.
    auto bindingDescription = Vertex::getBindingDescriptions( );
    auto attributeDescriptions = Vertex::getAttributeDescriptions( );

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo( { },                                                    // flags
                                                            static_cast<uint32_t>( bindingDescription.size( ) ),    // vertexBindingDescriptionCount
                                                            bindingDescription.data( ),                             // pVertexBindingDescriptions
                                                            static_cast<uint32_t>( attributeDescriptions.size( ) ), // vertexAttributeDescriptionCount
                                                            attributeDescriptions.data( ) );                        // pVertexAttributeDescriptions

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly( { },        // flags
                                                            topology,   // topology
                                                            VK_FALSE ); // primitiveRestartEnable

    vk::PipelineViewportStateCreateInfo viewportState( { },        // flags 
                                                       1,          // viewportCount
                                                       &viewport,  // pViewports
                                                       1,          // scissorCount
                                                       &scissor ); // pScissors

    vk::PipelineRasterizationStateCreateInfo rasterizer( { },                              // flags
                                                         VK_FALSE,                         // depthClampEnable
                                                         VK_FALSE,                         // rasterizerDiscardEnable
                                                         vk::PolygonMode::eFill,           // polygonMode
                                                         vk::CullModeFlagBits::eBack,      // cullMode
                                                         vk::FrontFace::eCounterClockwise, // frontFace
                                                         VK_FALSE,                         // depthBiasEnable
                                                         0.0f,                             // depthBiasConstantFactor
                                                         0.0f,                             // depthBiasClamp
                                                         0.0f,                             // depthBiasSlopeFactor
                                                         1.0f );                           // lineWidth

    vk::PipelineMultisampleStateCreateInfo multisampling( { },                         // flags
                                                          vk::SampleCountFlagBits::e1, // rasterizationSamples
                                                          VK_FALSE,                    // sampleShadingEnable
                                                          0.0f,                        // minSampleShading
                                                          nullptr,                     // pSampleMask
                                                          VK_FALSE,                    // alphaToCoverageEnable
                                                          VK_FALSE );                  // alphaToOneEnable

    vk::PipelineDepthStencilStateCreateInfo depthStencil( { },                  // flags
                                                          VK_TRUE,              // depthTestEnable
                                                          VK_TRUE,              // depthWriteEnable
                                                          vk::CompareOp::eLess, // depthCompareOp
                                                          VK_FALSE,             // depthBoundsTestEnable
                                                          VK_FALSE,             // stencilTestEnable
                                                          { },                  // front
                                                          { },                  // back
                                                          0.0f,                 // minDepthBounds
                                                          0.0f );               // maxDepthBounds

    vk::PipelineColorBlendAttachmentState colorBlendAttachment( VK_FALSE,                                                                                                                            // blendEnable
                                                                { },                                                                                                                                 // srcColorBlendFactor
                                                                { },                                                                                                                                 // dstColorBlendFactor
                                                                { },                                                                                                                                 // colorBlendOp
                                                                { },                                                                                                                                 // srcAlphaBlendFactor
                                                                { },                                                                                                                                 // dstAlphaBlendFactor
                                                                { },                                                                                                                                 // alphaBlendOp
                                                                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA ); // colorWriteMask
                                                                
    vk::PipelineColorBlendStateCreateInfo colorBlending( { },                          // flags
                                                         VK_FALSE,                     // logicOpEnable
                                                         vk::LogicOp::eCopy,           // logicOp)
                                                         1,                            // attachmentCount
                                                         &colorBlendAttachment,        // pAttachments
                                                         { 0.0f, 0.0f, 0.0f, 0.0f } ); // blendConstants

    std::array<vk::DynamicState, 2> dynamicStates { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

    vk::PipelineDynamicStateCreateInfo dynamicStateInfo( { },                                            // flags
                                                         static_cast<uint32_t>( dynamicStates.size( ) ), // dynamicStateCount
                                                         dynamicStates.data( ) );                        // pDynamicStates

    
    uint32_t pushConstantSize = sizeof( float ) + sizeof( glm::vec3 );
    RX_ASSERT( g_physicalDeviceLimits.maxPushConstantsSize >= pushConstantSize, "Push constant size is exceeding supported size." );

    vk::PushConstantRange pushConstantRange( vk::ShaderStageFlagBits::eFragment, // stageFlags
                                             0,                                  // offset
                                             pushConstantSize );                 // size

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo( { },                  // flags
                                                     1,                    // setLayoutCount
                                                     &descriptorSetLayout, // pSetLayouts
                                                     1,                    // pushConstantRangeCount
                                                     &pushConstantRange ); // pPushConstantRanges

    m_layout = g_device.createPipelineLayoutUnique( pipelineLayoutInfo );
    if ( !m_layout )
      RX_ERROR( "Failed to create pipeline layout." );

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eVertex, vertexShader );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eFragment, fragmentShader );

    vk::GraphicsPipelineCreateInfo createInfo( { },                                            // flags
                                               static_cast<uint32_t>( shaderStages.size( ) ) , // stageCount
                                               shaderStages.data( ),                           // pStages
                                               &vertexInputInfo,                               // pVertexInputState
                                               &inputAssembly,                                 // pInputAssemblyState
                                               nullptr,                                        // pTessellationState
                                               &viewportState,                                 // pViewportStage
                                               &rasterizer,                                    // pRasterizationState
                                               &multisampling,                                 // pMultisampleState
                                               &depthStencil,                                  // pDepthStencilState
                                               &colorBlending,                                 // pColorBlendState
                                               &dynamicStateInfo,                              // pDynamicState
                                               m_layout.get( ),                                // layout
                                               renderPass,                                     // renderPass
                                               0,                                              // subpass
                                               nullptr,                                        // basePipelineHandle
                                               0 );                                            // basePipelineIndex
  
    m_pipeline = g_device.createGraphicsPipelineUnique( nullptr, createInfo, nullptr );
    if ( !m_pipeline )
      RX_ERROR( "Failed to create graphics pipeline." );
  }

  void RasterizationPipeline::bind( vk::CommandBuffer commandBuffer ) const
  {
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, m_pipeline.get( ) );
  }

  RayTracingPipeline::RayTracingPipeline( vk::RenderPass renderPass,
                                          vk::Viewport viewport,
                                          vk::Rect2D scissor,
                                          const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                                          vk::ShaderModule rayGen,
                                          vk::ShaderModule miss,
                                          vk::ShaderModule closestHit,
                                          uint32_t maxRecursion,
                                          bool initialize )
  {
    if ( initialize )
      init( renderPass, viewport, scissor, descriptorSetLayouts, rayGen, miss, closestHit, maxRecursion );
  }

  void RayTracingPipeline::init( vk::RenderPass renderPass,
                                 vk::Viewport viewport,
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

  void RayTracingPipeline::bind( vk::CommandBuffer commandBuffer ) const
  {
    commandBuffer.bindPipeline( vk::PipelineBindPoint::eRayTracingKHR, m_pipeline.get( ) );
  }
}