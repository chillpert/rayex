#pragma once

#include "api/Bindings.hpp"
#include "api/RenderPass.hpp"
#include "api/image/Image.hpp"

namespace RAYEX_NAMESPACE
{
  /// The post processing renderer acts as a second render pass for enabling post processing operations, such as gamma correction.
  /// @ingroup API
  class PostProcessingRenderer
  {
  public:
    auto getRenderPass( ) const -> const RenderPass& { return _renderPass; }

    auto getPipeline( ) const -> const vk::Pipeline { return _pipeline.get( ); }

    auto getPipelineLayout( ) const -> const vk::PipelineLayout { return _pipelineLayout.get( ); }

    void initDepthImage( vk::Extent2D extent );

    void initRenderPass( vk::Format format );

    void initDescriptorSet( );

    /// @param imageInfo The descriptor image info of the path tracer's storage image.
    void updateDescriptors( vk::DescriptorImageInfo imageInfo );

    void initPipeline( );

    void beginRenderPass( vk::CommandBuffer commandBuffer, vk::Framebuffer framebuffer, vk::Extent2D size );

    void endRenderPass( vk::CommandBuffer commandBuffer );

    /// Records the draw calls to a given command buffer.
    /// @param imageIndex The current swapchain image index for selecting the correct descriptor set.
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
