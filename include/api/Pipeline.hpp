#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "base/Settings.hpp"
#include "pch/stdafx.hpp"

namespace RAYEX_NAMESPACE
{
  /// A wrapper class for a Vulkan graphics pipeline.
  /// @ingroup API
  class Pipeline
  {
  public:
    /// Initializes a rasterization pipeline.
    /// @param descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    /// @param renderPass A Vulkan render pass.
    /// @param viewport The desired view port.
    /// @param scissor The desired scissor dimensions.
    /// @param settings Used to retrieve information like maximum instances or lights of a specific type.
    void init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, vk::RenderPass renderPass, vk::Viewport viewport, vk::Rect2D scissor, const Settings* settings );

    /// @return Returns the Vulkan pipeline object without the unique handle.
    auto get( ) const -> vk::Pipeline { return _pipeline.get( ); }

    /// @return Returns the Vulkan pipeline layout object without the unique handle.
    auto getLayout( ) const -> vk::PipelineLayout { return _layout.get( ); }

  private:
    vk::UniquePipeline _pipeline;     ///< The Vulkan pipeline with a unique handle.
    vk::UniquePipelineLayout _layout; ///< The Vulkan pipeline layout with a unique handle.
  };
} // namespace RAYEX_NAMESPACE

#endif // PIPELINE_HPP
