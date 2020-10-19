#include "api/Pipeline.hpp"

#include "api/Components.hpp"
#include "api/Vertex.hpp"
#include "api/raytracing/RayTracingBuilder.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Util.hpp"

namespace RAYEX_NAMESPACE
{
  void Pipeline::init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, vk::RenderPass renderPass, vk::Viewport viewport, vk::Rect2D scissor, const Settings* settings )
  {
    // TODO: this has to be more adjustable.
    auto bindingDescription    = Vertex::getBindingDescriptions( );
    auto attributeDescriptions = Vertex::getAttributeDescriptions( );

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo( { },                                                    // flags
                                                            static_cast<uint32_t>( bindingDescription.size( ) ),    // vertexBindingDescriptionCount
                                                            bindingDescription.data( ),                             // pVertexBindingDescriptions
                                                            static_cast<uint32_t>( attributeDescriptions.size( ) ), // vertexAttributeDescriptionCount
                                                            attributeDescriptions.data( ) );                        // pVertexAttributeDescriptions

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly( { },                                  // flags
                                                            vk::PrimitiveTopology::eTriangleList, // topology
                                                            VK_FALSE );                           // primitiveRestartEnable

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
    if ( g_physicalDeviceLimits.maxPushConstantsSize < pushConstantSize )
      RX_FATAL( "Push constant size is exceeding supported size." );

    vk::PushConstantRange pushConstantRangeMissKHR( vk::ShaderStageFlagBits::eVertex, // stageFlags
                                                    0,                                // offset
                                                    sizeof( uint32_t ) );             // size

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo( { },                                                   // flags
                                                     static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                                     descriptorSetLayouts.data( ),                          // pSetLayouts
                                                     1,                                                     // pushConstantRangeCount
                                                     &pushConstantRangeMissKHR );                           // pPushConstantRanges

    this->layout = g_device.createPipelineLayoutUnique( pipelineLayoutInfo );
    RX_ASSERT( this->layout, "Failed to create pipeline layout for rasterization pipeline." );

    uint32_t anticipatedDirectionalLights = settings->maxDirectionalLights.has_value( ) ? settings->maxDirectionalLights.value( ) : g_maxDirectionalLights;
    uint32_t anticipatedPointLights       = settings->maxPointLights.has_value( ) ? settings->maxPointLights.value( ) : g_maxPointLights;
    Util::processShaderMacros( "shaders/simple3D.frag", anticipatedDirectionalLights, anticipatedPointLights, 0 );

    auto vs = vk::Initializer::initShaderModuleUnique( "shaders/simple3D.vert" );
    auto fs = vk::Initializer::initShaderModuleUnique( "shaders/simple3D.frag" );

    std::array<vk::SpecializationMapEntry, 1> mapEntries;
    mapEntries[0] = vk::SpecializationMapEntry( 0,                    // constantID
                                                0,                    // offset
                                                sizeof( uint32_t ) ); // size

    std::array<uint32_t, 1> data;
    data[0] = anticipatedDirectionalLights;

    vk::SpecializationInfo specializationInfo( static_cast<uint32_t>( mapEntries.size( ) ),            // mapEntryCount
                                               mapEntries.data( ),                                     // pMapEntries
                                               sizeof( data ) * static_cast<uint32_t>( data.size( ) ), // dataSize
                                               data.data( ) );                                         // pData

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
    shaderStages[0]                     = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eVertex, vs.get( ) );
    shaderStages[1]                     = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eFragment, fs.get( ) );
    shaderStages[1].pSpecializationInfo = &specializationInfo;

    vk::GraphicsPipelineCreateInfo createInfo( { },                                           // flags
                                               static_cast<uint32_t>( shaderStages.size( ) ), // stageCount
                                               shaderStages.data( ),                          // pStages
                                               &vertexInputInfo,                              // pVertexInputState
                                               &inputAssembly,                                // pInputAssemblyState
                                               nullptr,                                       // pTessellationState
                                               &viewportState,                                // pViewportStage
                                               &rasterizer,                                   // pRasterizationState
                                               &multisampling,                                // pMultisampleState
                                               &depthStencil,                                 // pDepthStencilState
                                               &colorBlending,                                // pColorBlendState
                                               &dynamicStateInfo,                             // pDynamicState
                                               layout.get( ),                                 // layout
                                               renderPass,                                    // renderPass
                                               0,                                             // subpass
                                               nullptr,                                       // basePipelineHandle
                                               0 );                                           // basePipelineIndex

    this->pipeline = g_device.createGraphicsPipelineUnique( nullptr, createInfo, nullptr );
    RX_ASSERT( this->pipeline, "Failed to create rasterization pipeline." );
  }

  void Pipeline::init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, const Settings* settings )
  {
    uint32_t anticipatedDirectionalLights = settings->maxDirectionalLights.has_value( ) ? settings->maxDirectionalLights.value( ) : g_maxDirectionalLights;
    uint32_t anticipatedPointLights       = settings->maxPointLights.has_value( ) ? settings->maxPointLights.value( ) : g_maxPointLights;
    Util::processShaderMacros( "shaders/raytrace.rchit", anticipatedDirectionalLights, anticipatedPointLights, g_modelCount );

    auto rgen       = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rgen" );
    auto miss       = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rmiss" );
    auto chit       = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rchit" );
    auto missShadow = vk::Initializer::initShaderModuleUnique( "shaders/raytraceShadow.rmiss" );

    vk::PushConstantRange rtPushConstant( vk::ShaderStageFlagBits::eRaygenKHR | vk::ShaderStageFlagBits::eMissKHR | vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                          0,                                                                                                                 // offset
                                          sizeof( RayTracePushConstants ) );                                                                                 // size

    std::array<vk::PushConstantRange, 1> pushConstantRanges = { rtPushConstant };

    vk::PipelineLayoutCreateInfo layoutInfo( { },                                                   // flags
                                             static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                             descriptorSetLayouts.data( ),                          // pSetLayouts
                                             static_cast<uint32_t>( pushConstantRanges.size( ) ),   // pushConstantRangeCount
                                             pushConstantRanges.data( ) );                          // pPushConstantRanges

    this->layout = g_device.createPipelineLayoutUnique( layoutInfo );
    RX_ASSERT( this->layout, "Failed to create pipeline layout for ray tracing pipeline." );

    std::array<vk::PipelineShaderStageCreateInfo, 4> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eRaygenKHR, rgen.get( ) );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, miss.get( ) );
    shaderStages[2] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, missShadow.get( ) );
    shaderStages[3] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eClosestHitKHR, chit.get( ) );

    // Set up raytracing shader groups.
    std::array<vk::RayTracingShaderGroupCreateInfoKHR, 4> groups;

    for ( auto& group : groups )
    {
      group.generalShader      = VK_SHADER_UNUSED_KHR;
      group.closestHitShader   = VK_SHADER_UNUSED_KHR;
      group.anyHitShader       = VK_SHADER_UNUSED_KHR;
      group.intersectionShader = VK_SHADER_UNUSED_KHR;
    }

    groups[0].generalShader = 0;
    groups[0].type          = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[1].generalShader = 1;
    groups[1].type          = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[2].generalShader = 2;
    groups[2].type          = vk::RayTracingShaderGroupTypeKHR::eGeneral;

    groups[3].closestHitShader = 3;
    groups[3].type             = vk::RayTracingShaderGroupTypeKHR::eTrianglesHitGroup;

    g_shaderGroups = static_cast<uint32_t>( groups.size( ) );

    vk::RayTracingPipelineCreateInfoKHR createInfo( { },                                           // flags
                                                    static_cast<uint32_t>( shaderStages.size( ) ), // stageCount
                                                    shaderStages.data( ),                          // pStages
                                                    static_cast<uint32_t>( groups.size( ) ),       // groupCount
                                                    groups.data( ),                                // pGroups
                                                    settings->getRecursionDepth( ),                // maxRecursionDepth
                                                    0,                                             // libraries
                                                    nullptr,                                       // pLibraryInterface
                                                    this->layout.get( ),                           // layout
                                                    nullptr,                                       // basePipelineHandle
                                                    0 );                                           // basePipelineIndex

    this->pipeline = g_device.createRayTracingPipelineKHRUnique( nullptr, createInfo );
    RX_ASSERT( this->pipeline, "Failed to create ray tracing pipeline." );
  }
} // namespace RAYEX_NAMESPACE
