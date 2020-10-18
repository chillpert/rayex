#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Settings.hpp"
#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  struct RayTracePushConstants
  {
    glm::vec4 clearColor         = glm::vec4( 1.0F );
    uint32_t sampleRate          = 100;
    uint32_t sampleRatePerRayGen = 1;
    uint32_t ssaa                = 8;
    uint32_t jitterCamEnabled    = 0;
    uint32_t ssaaEnabled         = 1;

    float padding0 = 0.0f;
    float padding1 = 0.0f;
    float padding2 = 0.0f;
  };

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

    /// Initializes a ray tracing pipeline.
    /// @param descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    /// @param settings Used to retrieve information like maximum instances or lights of a specific type.
    void init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, const Settings* settings );

    /// @return Returns the Vulkan pipeline object without the unique handle.
    auto get( ) const -> vk::Pipeline { return this->pipeline.get( ); }

    /// @return Returns the Vulkan pipeline layout object without the unique handle.
    auto getLayout( ) const -> vk::PipelineLayout { return this->layout.get( ); }

  private:
    vk::UniquePipeline pipeline;     ///< The Vulkan pipeline with a unique handle.
    vk::UniquePipelineLayout layout; ///< The Vulkan pipeline layout with a unique handle.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // PIPELINE_HPP
