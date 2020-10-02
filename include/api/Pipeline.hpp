#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "pch/stdafx.hpp"
#include "Settings.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A wrapper class for a Vulkan graphics pipeline.
  /// @ingroup API
  class Pipeline
  {
  public:
    /// Initializes a rasterization pipeline.
    /// @renderPass A Vulkan render pass.
    /// @descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    void init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, vk::RenderPass renderPass, vk::Viewport viewport, vk::Rect2D scissor, const Settings* const settings );

    /// Initializes a ray tracing pipeline.
    /// @param descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    void init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, const Settings* const settings );

    /// @return Returns the Vulkan pipeline object without the unique handle.
    inline const vk::Pipeline get( ) const { return this->pipeline.get( ); }

    /// @return Returns the Vulkan pipeline layout object without the unique handle.
    inline const vk::PipelineLayout getLayout( ) const { return this->layout.get( ); }

  private:
    vk::UniquePipeline pipeline; ///< The Vulkan pipeline with a unique handle.
    vk::UniquePipelineLayout layout; ///< The Vulkan pipeline layout with a unique handle.
  };
}

#endif // PIPELINE_HPP