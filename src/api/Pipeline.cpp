#include "Pipeline.hpp"
#include "Vertex.hpp"
#include "Helpers.hpp"
#include "Components.hpp"

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

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast< uint32_t >( bindingDescription.size( ) );
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast< uint32_t >( attributeDescriptions.size( ) );
    vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data( );
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data( );

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = topology;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport; // TODO: spec states if the viewport is dynamic this is ignored.
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = vk::CompareOp::eLess;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::array<vk::DynamicState, 2> dynamicStates{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };

    vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
    dynamicStateInfo.dynamicStateCount = static_cast< uint32_t >( dynamicStates.size( ) );
    dynamicStateInfo.pDynamicStates = dynamicStates.data( );

    vk::PushConstantRange pushConstantRange;
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof( glm::vec3 );

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    m_layout = g_device.createPipelineLayoutUnique( pipelineLayoutInfo );
    if ( !m_layout )
      RX_ERROR( "Failed to create pipeline layout." );

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = vertexShader;
    vertShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = fragmentShader;
    fragShaderStageInfo.pName = "main";

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

    vk::GraphicsPipelineCreateInfo createInfo;
    createInfo.stageCount = static_cast< uint32_t >( shaderStages.size( ) );
    createInfo.pStages = shaderStages.data( );
    createInfo.pVertexInputState = &vertexInputInfo;
    createInfo.pInputAssemblyState = &inputAssembly;
    createInfo.pViewportState = &viewportState;
    createInfo.pRasterizationState = &rasterizer;
    createInfo.pMultisampleState = &multisampling;
    createInfo.pDepthStencilState = &depthStencil;
    createInfo.pColorBlendState = &colorBlending;
    createInfo.layout = m_layout.get( );
    createInfo.renderPass = renderPass;
    createInfo.subpass = 0;
    createInfo.basePipelineHandle = nullptr;
    createInfo.pDynamicState = &dynamicStateInfo;

    m_pipeline = g_device.createGraphicsPipelineUnique( nullptr, createInfo, nullptr );
    if ( !m_pipeline )
      RX_ERROR( "Failed to create graphics pipeline." );
  }

  RaytracingPipeline::RaytracingPipeline( vk::RenderPass renderPass,
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

  void RaytracingPipeline::init( vk::RenderPass renderPass,
                                 vk::Viewport viewport,
                                 vk::Rect2D scissor,
                                 const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                                 vk::ShaderModule rayGen,
                                 vk::ShaderModule miss,
                                 vk::ShaderModule closestHit,
                                 uint32_t maxRecursion )
  {
    vk::PipelineLayoutCreateInfo layoutInfo
    {
      { },                                                // flags
      static_cast< uint32_t >( descriptorSetLayouts.size( ) ), // setLayoutCount
      descriptorSetLayouts.data( ),                        // pSetLayouts
      0,                                                  // pushConstantRangeCount
      nullptr                                             // pPushConstantRanges
    };

    m_layout = g_device.createPipelineLayoutUnique( layoutInfo );
    if ( !m_layout )
      RX_ERROR( "Failed to create pipeline layout." );

    vk::SpecializationMapEntry specializationMapEntry
    {
      0,                // constantID
      0,                // offset
      sizeof( uint32_t )  // size
    };

    vk::SpecializationInfo specializationInfo
    {
       1,                         // mapEntryCount
       &specializationMapEntry,   // pMapEntries
       sizeof( maxRecursion ),      // dataSize
       &maxRecursion              // pData
    };

    std::array<vk::PipelineShaderStageCreateInfo, 3> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eRaygenKHR, rayGen );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, miss );
    shaderStages[2] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eClosestHitKHR, closestHit );

    // Set up raytracing shader groups.
    std::array<vk::RayTracingShaderGroupCreateInfoKHR, 3> groups;

    groups[0].generalShader = 0;

    groups[1].generalShader = 1;

    groups[2].closestHitShader = 2;

    vk::RayTracingPipelineCreateInfoKHR createInfo{
      { },                                        // flags
      static_cast< uint32_t >( shaderStages.size( ) ), // stageCount
      shaderStages.data( ),                        // pStages
      static_cast< uint32_t >( groups.size( ) ),       // groupCount
      groups.data( ),                              // pGroups
      maxRecursion,                               // maxRecursionDepth
      0,                                          // libraries
      nullptr,                                    // pLibraryInterface
      m_layout.get( ),                             // layout
      nullptr,                                    // basePipelineHandle
      0                                           // basePipelineIndex
    };

    auto result = g_device.createRayTracingPipelineKHR( nullptr, createInfo, nullptr, *g_dispatchLoaderDynamic );

    if ( result.result != vk::Result::eSuccess )
      RX_ERROR( "Failed to create ray tracing pipeline." );

    m_pipeline.get( ) = result.value;
  }
}
