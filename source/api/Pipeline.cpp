#include "api/Pipeline.hpp"
#include "api/misc/Vertex.hpp"
#include "api/utility/Helpers.hpp"
#include "api/misc/Components.hpp"
#include "api/raytracing/RayTracingBuilder.hpp"
#include "api/utility/Initializers.hpp"
#include "api/utility/Util.hpp"

namespace RAYEXEC_NAMESPACE
{
  bool Pipeline::init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, vk::RenderPass renderPass, vk::Viewport viewport, vk::Rect2D scissor )
  {
    // TODO: this has to be more adjustable.
    auto bindingDescription = Vertex::getBindingDescriptions( );
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
    {
      RX_ERROR( "Push constant size is exceeding supported size." );
      return false;
    }

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo( { },                                                   // flags
                                                     static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                                     descriptorSetLayouts.data( ),                          // pSetLayouts
                                                     0,                                                     // pushConstantRangeCount
                                                     nullptr );                                             // pPushConstantRanges

    this->layout = g_device.createPipelineLayoutUnique( pipelineLayoutInfo );
    if ( !this->layout )
    {
      RX_ERROR( "Failed to create pipeline layout for rasterization pipeline." );
      return false;
    }

    auto vs = vk::Initializer::initShaderModuleUnique( "shaders/simple3D.vert" );
    auto fs = vk::Initializer::initShaderModuleUnique( "shaders/simple3D.frag" );

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eVertex, vs.get( ) );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eFragment, fs.get( ) );

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
    if ( !this->pipeline )
    {
      RX_ERROR( "Failed to create rasterization pipeline." );
      return false;
    }
    
    return true;
  }

  bool Pipeline::init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, const Settings* const settings )
  {
    Util::processShaderMacros( "shaders/raytrace.rchit", settings->anticipatedDirectionalLights, settings->anticipatedPointLights );

    auto rgen = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rgen" );
    auto miss = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rmiss" );
    auto chit = vk::Initializer::initShaderModuleUnique( "shaders/raytrace.rchit" );

    vk::PushConstantRange pushConstantRangeMissKHR( vk::ShaderStageFlagBits::eMissKHR, // stageFlags
                                                    0,                                 // offset
                                                    sizeof( glm::vec4 ) );             // size

    vk::PushConstantRange pushConstantRangeClosestHitKHR( vk::ShaderStageFlagBits::eClosestHitKHR, // stageFlags
                                                          sizeof( glm::vec4 ),                     // offset
                                                          sizeof( glm::vec4 ) );                   // size

    std::array<vk::PushConstantRange, 2> pushConstantRanges = { pushConstantRangeMissKHR, pushConstantRangeClosestHitKHR };

    vk::PipelineLayoutCreateInfo layoutInfo( { },                                                   // flags
                                             static_cast<uint32_t>( descriptorSetLayouts.size( ) ), // setLayoutCount
                                             descriptorSetLayouts.data( ),                          // pSetLayouts
                                             static_cast<uint32_t>( pushConstantRanges.size( ) ),   // pushConstantRangeCount
                                             pushConstantRanges.data( ) );                          // pPushConstantRanges

    this->layout = g_device.createPipelineLayoutUnique( layoutInfo );
    if ( !this->layout )
    {
      RX_ERROR( "Failed to create pipeline layout for ray tracing pipeline." );
      return false;
    }

    std::array<vk::PipelineShaderStageCreateInfo, 3> shaderStages;
    shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eRaygenKHR, rgen.get( ) );
    shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eMissKHR, miss.get( ) );
    shaderStages[2] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eClosestHitKHR, chit.get( ) );

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
                                                    settings->getRecursionDepth( ),                // maxRecursionDepth
                                                    0,                                             // libraries
                                                    nullptr,                                       // pLibraryInterface
                                                    this->layout.get( ),                           // layout
                                                    nullptr,                                       // basePipelineHandle
                                                    0 );                                           // basePipelineIndex
  
    this->pipeline = g_device.createRayTracingPipelineKHRUnique( nullptr, createInfo );
    if ( !this->pipeline )
    {
      RX_ERROR( "Failed to create ray tracing pipeline." );
      return false;
    }

    return true;
  }
}