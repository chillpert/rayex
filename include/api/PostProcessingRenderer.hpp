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

    void initDepthImage( vk::Extent2D extent );

    void initRenderPass( vk::Format format );

    void initDescriptorSet( );

    void updateDescriptors( vk::DescriptorImageInfo imageInfo );

    void initPipeline( );

    void beginRenderPass( vk::CommandBuffer commandBuffer, vk::Framebuffer framebuffer, vk::Extent2D size );

    void endRenderPass( vk::CommandBuffer commandBuffer );

    void render( vk::CommandBuffer commandBuffer, vk::Extent2D size, size_t imageIndex );

  private:
    Image _depthImage;
    vk::UniqueImageView _depthImageView;

    RenderPass _renderPass;

    vk::UniquePipeline _pipeline;
    vk::UniquePipelineLayout _pipelineLayout;

    Descriptors _descriptors;
    std::vector<vk::DescriptorSet> _descriptorSets;
  };
} // namespace RAYEX_NAMESPACE
