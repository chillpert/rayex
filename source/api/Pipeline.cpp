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
    if ( components::physicalDeviceLimits.maxPushConstantsSize < pushConstantSize )
      RX_FATAL( "Push constant size is exceeding supported size." );

    vk::PushConstantRange pushConstantRangeMissKHR( vk::ShaderStageFlagBits::eVertex, // stageFlags
                                                    0,                                // offset
                                                    sizeof( uint32_t ) );             // size

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo( { },                                                   // flags
                                                     static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                                     descriptorSetLayouts.data( ),                          // pSetLayouts
                                                     1,                                                     // pushConstantRangeCount
                                                     &pushConstantRangeMissKHR );                           // pPushConstantRanges

    _layout = components::device.createPipelineLayoutUnique( pipelineLayoutInfo );
    RX_ASSERT( _layout, "Failed to create pipeline layout for rasterization pipeline." );

    //uint32_t anticipatedDirectionalLights = settings->maxDirectionalLights.has_value( ) ? settings->maxDirectionalLights.value( ) : components::maxDirectionalLights;
    //uint32_t anticipatedPointLights       = settings->maxPointLights.has_value( ) ? settings->maxPointLights.value( ) : components::maxPointLights;
    //Util::processShaderMacros( "shaders/simple3D.frag", anticipatedDirectionalLights, anticipatedPointLights, 0 );

    auto vs = vk::Initializer::initShaderModuleUnique( "shaders/simple3D.vert" );
    auto fs = vk::Initializer::initShaderModuleUnique( "shaders/simple3D.frag" );

    std::array<vk::SpecializationMapEntry, 1> mapEntries;
    mapEntries[0] = vk::SpecializationMapEntry( 0,                    // constantID
                                                0,                    // offset
                                                sizeof( uint32_t ) ); // size

    /*
    std::array<uint32_t, 1> data;
    data[0] = anticipatedDirectionalLights;

    vk::SpecializationInfo specializationInfo( static_cast<uint32_t>( mapEntries.size( ) ),            // mapEntryCount
                                               mapEntries.data( ),                                     // pMapEntries
                                               sizeof( data ) * static_cast<uint32_t>( data.size( ) ), // dataSize
                                               data.data( ) );                                         // pData
    */

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eVertex, vs.get( ) );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eFragment, fs.get( ) );
    //shaderStages[1].pSpecializationInfo = &specializationInfo;

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
                                               _layout.get( ),                                // layout
                                               renderPass,                                    // renderPass
                                               0,                                             // subpass
                                               nullptr,                                       // basePipelineHandle
                                               0 );                                           // basePipelineIndex

    _pipeline = components::device.createGraphicsPipelineUnique( nullptr, createInfo, nullptr );
    RX_ASSERT( _pipeline, "Failed to create rasterization pipeline." );
  }
} // namespace RAYEX_NAMESPACE
