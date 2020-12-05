#pragma once

#include "api/Bindings.hpp"
#include "api/Components.hpp"
#include "api/RenderPass.hpp"
#include "api/Swapchain.hpp"
#include "api/image/Image.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Util.hpp"

namespace RAYEX_NAMESPACE
{
  class PostProcessingRenderer
  {
  public:
    auto getRenderPass( ) const -> const RenderPass& { return _renderPass; }

    auto getPipeline( ) const -> const vk::Pipeline { return _pipeline.get( ); }

    auto getPipelineLayout( ) const -> const vk::PipelineLayout { return _pipelineLayout.get( ); }

    // @todo move to path tracer
    void initColorImage( vk::Extent2D extent )
    {
      auto samplerCreateInfo = vk::Helper::getSamplerCreateInfo( );
      _colorImageSampler     = vk::Initializer::initSamplerUnique( samplerCreateInfo );
    }

    void initDepthImage( vk::Extent2D extent )
    {
      vk::Format format = getSupportedDepthFormat( components::physicalDevice );

      auto imageCreateInfo   = vk::Helper::getImageCreateInfo( vk::Extent3D( extent, 1 ) );
      imageCreateInfo.format = format;
      imageCreateInfo.usage  = vk::ImageUsageFlagBits::eDepthStencilAttachment;
      _depthImage.init( imageCreateInfo );

      auto imageViewCreateInfo = vk::Helper::getImageViewCreateInfo( _depthImage.get( ), format, vk::ImageViewType::e2D, vk::ImageAspectFlagBits::eDepth );
      _depthImageView          = vk::Initializer::initImageViewUnique( imageViewCreateInfo );

      vk::ImageSubresourceRange subresourceRange( vk::ImageAspectFlagBits::eDepth, // aspectMask
                                                  0,                               // baseMipLevel
                                                  VK_REMAINING_MIP_LEVELS,         // levelCount
                                                  0,                               // baseArrayLayer
                                                  VK_REMAINING_ARRAY_LAYERS );     // layerCount

      _depthImage.transitionToLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal, &subresourceRange );
    }

    void initRenderPass( vk::Format format )
    {
      // Color attachment
      vk::AttachmentDescription colorAttachmentDescription( { },                               // flags
                                                            format,                            // format
                                                            vk::SampleCountFlagBits::e1,       // samples
                                                            vk::AttachmentLoadOp::eClear,      // loadOp
                                                            { },                               // storeOp
                                                            { },                               // stencilLoadOp
                                                            { },                               // stencilStoreOp
                                                            { },                               // initialLayout
                                                            vk::ImageLayout::ePresentSrcKHR ); // finalLayout

      // Depth attachment
      vk::AttachmentDescription depthAttachmentDescription( { },                                                   // flags
                                                            getSupportedDepthFormat( components::physicalDevice ), // format
                                                            vk::SampleCountFlagBits::e1,                           // samples
                                                            vk::AttachmentLoadOp::eClear,                          // loadOp
                                                            { },                                                   // storeOp
                                                            vk::AttachmentLoadOp::eClear,                          // stencilLoadOp
                                                            { },                                                   // stencilStoreOp
                                                            { },                                                   // initialLayout
                                                            vk::ImageLayout::eDepthStencilAttachmentOptimal );     // finalLayout

      vk::AttachmentReference colorAttachmentReference( 0,                                          // attachment
                                                        vk::ImageLayout::eColorAttachmentOptimal ); // layout

      vk::AttachmentReference depthAttachmentRef( 1,                                                 // attachment
                                                  vk::ImageLayout::eDepthStencilAttachmentOptimal ); // layout

      vk::SubpassDescription subpassDescription( { },                              // flags
                                                 vk::PipelineBindPoint::eGraphics, // pipelineBindPoint
                                                 0,                                // inputAttachmentsCount
                                                 nullptr,                          // pInputAttachments
                                                 1,                                // colorAttachmentsCount
                                                 &colorAttachmentReference,        // pColorAttachments
                                                 nullptr,                          // pResolveAttachments
                                                 &depthAttachmentRef,              // pDepthStencilAttachment
                                                 0,                                // preserveAttachemntCount
                                                 nullptr );                        // pPreserveAttachments

      vk::SubpassDependency subpassDependencies( VK_SUBPASS_EXTERNAL,                                                                  // srcSubpass
                                                 0,                                                                                    // dstSubpass
                                                 vk::PipelineStageFlagBits::eBottomOfPipe,                                             // srcStageMask
                                                 vk::PipelineStageFlagBits::eColorAttachmentOutput,                                    // dstStageMask
                                                 vk::AccessFlagBits::eMemoryRead,                                                      // srcAccessMask
                                                 vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite, // dstAccessMask
                                                 vk::DependencyFlagBits::eByRegion );                                                  // dependencyFlags

      _renderPass.init( { colorAttachmentDescription, depthAttachmentDescription }, { subpassDescription }, { subpassDependencies } );
    }

    void initDescriptorSet( )
    {
      // Color image binding
      _descriptors.bindings.add( 0,
                                 vk::DescriptorType::eCombinedImageSampler,
                                 vk::ShaderStageFlagBits::eFragment );

      _descriptors.layout = _descriptors.bindings.initLayoutUnique( );
      _descriptors.pool   = _descriptors.bindings.initPoolUnique( components::swapchainImageCount );
      _descriptorSets     = vk::Initializer::initDescriptorSetsUnique( _descriptors.pool, _descriptors.layout );
    }

    void updateDescriptors( vk::ImageView imageView, vk::ImageLayout layout )
    {
      vk::DescriptorImageInfo imageInfo( _colorImageSampler.get( ), // sampler
                                         imageView,                 // imageView
                                         layout );                  // imageLayout

      _descriptors.bindings.write( _descriptorSets, 0, &imageInfo );
      _descriptors.bindings.update( );
    }

    void initPipeline( vk::Viewport viewport, vk::Rect2D scissor )
    {
      vk::PipelineVertexInputStateCreateInfo vertexInputInfo( { },       // flags
                                                              0,         // vertexBindingDescriptionCount
                                                              nullptr,   // pVertexBindingDescriptions
                                                              0,         // vertexAttributeDescriptionCount
                                                              nullptr ); // pVertexAttributeDescriptions

      vk::PipelineInputAssemblyStateCreateInfo inputAssembly( { },                                  // flags
                                                              vk::PrimitiveTopology::eTriangleList, // topology
                                                              VK_FALSE );                           // primitiveRestartEnable

      vk::PipelineViewportStateCreateInfo viewportState( { },       // flags
                                                         1,         // viewportCount
                                                         nullptr,   // pViewports
                                                         1,         // scissorCount
                                                         nullptr ); // pScissors

      vk::PipelineRasterizationStateCreateInfo rasterizer( { },                              // flags
                                                           VK_FALSE,                         // depthClampEnable
                                                           VK_FALSE,                         // rasterizerDiscardEnable
                                                           vk::PolygonMode::eFill,           // polygonMode
                                                           vk::CullModeFlagBits::eNone,      // cullMode
                                                           vk::FrontFace::eCounterClockwise, // frontFace
                                                           VK_FALSE,                         // depthBiasEnable
                                                           0.0F,                             // depthBiasConstantFactor
                                                           0.0F,                             // depthBiasClamp
                                                           0.0F,                             // depthBiasSlopeFactor
                                                           1.0F );                           // lineWidth

      vk::PipelineMultisampleStateCreateInfo multisampling( { },                         // flags
                                                            vk::SampleCountFlagBits::e1, // rasterizationSamples
                                                            VK_FALSE,                    // sampleShadingEnable
                                                            0.0F,                        // minSampleShading
                                                            nullptr,                     // pSampleMask
                                                            VK_FALSE,                    // alphaToCoverageEnable
                                                            VK_FALSE );                  // alphaToOneEnable

      vk::PipelineDepthStencilStateCreateInfo depthStencil( { },                         // flags
                                                            VK_TRUE,                     // depthTestEnable
                                                            VK_TRUE,                     // depthWriteEnable
                                                            vk::CompareOp::eLessOrEqual, // depthCompareOp
                                                            VK_FALSE,                    // depthBoundsTestEnable
                                                            VK_FALSE,                    // stencilTestEnable
                                                            { },                         // front
                                                            { },                         // back
                                                            0.0F,                        // minDepthBounds
                                                            0.0F );                      // maxDepthBounds

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
                                                           vk::LogicOp::eClear,          // logicOp)
                                                           1,                            // attachmentCount
                                                           &colorBlendAttachment,        // pAttachments
                                                           { 0.0F, 0.0F, 0.0F, 0.0F } ); // blendConstants

      std::array<vk::DynamicState, 2> dynamicStates { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

      vk::PipelineDynamicStateCreateInfo dynamicStateInfo( { },                                            // flags
                                                           static_cast<uint32_t>( dynamicStates.size( ) ), // dynamicStateCount
                                                           dynamicStates.data( ) );                        // pDynamicStates

      vk::PushConstantRange pushConstantRange( vk::ShaderStageFlagBits::eFragment, // stageFlags
                                               0,                                  // offset
                                               sizeof( float ) );                  // size

      vk::PipelineLayoutCreateInfo layoutCreateInfo( { },                         // flags
                                                     1,                           // setLayoutCount
                                                     &_descriptors.layout.get( ), // pSetLayouts
                                                     1,                           // pushConstantRangeCount
                                                     &pushConstantRange );        // pPushConstantRanges

      _pipelineLayout = components::device.createPipelineLayoutUnique( layoutCreateInfo );
      RX_ASSERT( _pipelineLayout.get( ), "Failed to create pipeline layout for post processing renderer." );

      auto vert = vk::Initializer::initShaderModuleUnique( "shaders/PostProcessing.vert" );
      auto frag = vk::Initializer::initShaderModuleUnique( "shaders/PostProcessing.frag" );

      std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
      shaderStages[0] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eVertex, vert.get( ) );
      shaderStages[1] = vk::Helper::getPipelineShaderStageCreateInfo( vk::ShaderStageFlagBits::eFragment, frag.get( ) );

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
                                                 _pipelineLayout.get( ),                        // layout
                                                 _renderPass.get( ),                            // renderPass
                                                 0,                                             // subpass
                                                 nullptr,                                       // basePipelineHandle
                                                 0 );                                           // basePipelineIndex

      _pipeline = components::device.createGraphicsPipelineUnique( nullptr, createInfo, nullptr );
      RX_ASSERT( _pipeline.get( ), "Failed to create rasterization pipeline." );
    }

    void beginRenderPass( vk::CommandBuffer commandBuffer, vk::Framebuffer framebuffer, vk::Extent2D size )
    {
      std::array<vk::ClearValue, 2> clearValues;
      clearValues[0].color        = { Util::vec4toArray( glm::vec4( 1.0F, 1.0F, 1.0F, 1.0F ) ) };
      clearValues[1].depthStencil = vk::ClearDepthStencilValue { 1.0F, 0 };

      _renderPass.begin( framebuffer, commandBuffer, { 0, vk::Extent2D( size.width, size.height ) }, { clearValues[0], clearValues[1] } );
    }

    void endRenderPass( vk::CommandBuffer commandBuffer )
    {
      _renderPass.end( commandBuffer );
    }

    void render( vk::CommandBuffer commandBuffer, vk::Extent2D size, size_t imageIndex )
    {
      auto width  = static_cast<float>( size.width );
      auto height = static_cast<float>( size.height );

      commandBuffer.setViewport( 0, { vk::Viewport( 0.0F, 0.0F, width, height, 0.0F, 1.0F ) } );
      commandBuffer.setScissor( 0, { { { 0, 0 }, size } } );

      auto aspectRatio = width / height;
      commandBuffer.pushConstants( _pipelineLayout.get( ),             // layout
                                   vk::ShaderStageFlagBits::eFragment, // stageFlags
                                   0,                                  // offset
                                   sizeof( float ),                    // size
                                   &aspectRatio );                     // pValues

      commandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline.get( ) );

      commandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, // pipelineBindPoint
                                        _pipelineLayout.get( ),           // layout
                                        0,                                // first set
                                        1,                                // descriptor set count
                                        &_descriptorSets[imageIndex],     // descriptor sets
                                        0,                                // dynamic offset count
                                        nullptr );                        // dynamic offsets

      commandBuffer.draw( 3, 1, 0, 0 );
    }

  private:
    Image _depthImage;
    vk::UniqueImageView _depthImageView;

    //Image _colorImage;
    //vk::UniqueImageView _colorImageView;
    vk::UniqueSampler _colorImageSampler;

    RenderPass _renderPass;

    vk::UniquePipeline _pipeline;
    vk::UniquePipelineLayout _pipelineLayout;

    Descriptors _descriptors;
    std::vector<vk::DescriptorSet> _descriptorSets;
  };
} // namespace RAYEX_NAMESPACE
