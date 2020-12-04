#pragma once

#include "api/Components.hpp"
#include "api/RenderPass.hpp"
#include "api/Swapchain.hpp"
#include "api/image/Image.hpp"
#include "api/utility/Helpers.hpp"

namespace RAYEX_NAMESPACE
{
  class PostProcessingRenderer
  {
  public:
    void init( );

    auto getRenderPass( ) const -> const RenderPass& { return _renderPass; }

    auto getPipeline( ) const -> const vk::Pipeline { return _pipeline.get( ); }

    auto getPipelineLayout( ) const -> const vk::PipelineLayout { return _pipelineLayout.get( ); }

    void initColorImage( vk::Extent2D extent )
    {
      vk::Format format = vk::Format::eR32G32B32A32Sfloat;

      auto imageCreateInfo   = vk::Helper::getImageCreateInfo( vk::Extent3D( extent, 1 ) );
      imageCreateInfo.format = format;
      imageCreateInfo.usage  = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eStorage;
      _colorImage.init( imageCreateInfo );

      auto imageViewCreateInfo = vk::Helper::getImageViewCreateInfo( _colorImage.get( ), format );
      _colorImageView          = vk::Initializer::initImageViewUnique( imageViewCreateInfo );

      _colorImage.transitionToLayout( vk::ImageLayout::eGeneral );
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
      auto colorAttachmentDescription = vk::Helper::getAttachmentDescription( format );

      vk::AttachmentReference colorAttachmentReference( 0,                                          // attachment
                                                        vk::ImageLayout::eColorAttachmentOptimal ); // layout

      auto depthAttachmentDescription = vk::Helper::getDepthAttachmentDescription( getSupportedDepthFormat( components::physicalDevice ) );

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

      std::vector<vk::SubpassDependency> subpassDependencies( 2 );

      subpassDependencies[0] = { VK_SUBPASS_EXTERNAL,                                                                  // srcSubpass
                                 0,                                                                                    // dstSubpass
                                 vk::PipelineStageFlagBits::eBottomOfPipe,                                             // srcStageMask
                                 vk::PipelineStageFlagBits::eColorAttachmentOutput,                                    // dstStageMask
                                 vk::AccessFlagBits::eMemoryRead,                                                      // srcAccessMask
                                 vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite, // dstAccessMask
                                 vk::DependencyFlagBits::eByRegion };                                                  // dependencyFlags

      subpassDependencies[1] = { 0,                                                                                    // srcSubpass
                                 VK_SUBPASS_EXTERNAL,                                                                  // dstSubpass
                                 vk::PipelineStageFlagBits::eColorAttachmentOutput,                                    // srcStageMask
                                 vk::PipelineStageFlagBits::eBottomOfPipe,                                             // dstStageMask
                                 vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite, // srcAccessMask
                                 vk::AccessFlagBits::eMemoryRead,                                                      // dstAccessMask
                                 vk::DependencyFlagBits::eByRegion };                                                  // dependencyFlags

      _renderPass.init( { colorAttachmentDescription, depthAttachmentDescription }, { subpassDescription }, subpassDependencies );
    }

    void initPipeline( )
    {
      // Create the post processing pipeline
      vk::PipelineLayoutCreateInfo layoutCreateInfo( { },   // flags
                                                     1,     // setLayoutCount
                                                     { },   // pSetLayouts
                                                     { },   // pushConstantRangeCount
                                                     { } ); // pPushConstantRanges
    }

  private:
    Image _depthImage;
    vk::UniqueImageView _depthImageView;
    Image _colorImage;
    vk::UniqueImageView _colorImageView;

    RenderPass _renderPass;

    vk::UniquePipeline _pipeline;
    vk::UniquePipelineLayout _pipelineLayout;
  };
} // namespace RAYEX_NAMESPACE
