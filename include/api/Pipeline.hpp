#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Settings.hpp"
#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  enum class PipelineType
  {
    eDefaultRasterization,
    eDefaultRayTracing
  };

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
    /// @renderPass A Vulkan render pass.
    /// @descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    void init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, vk::RenderPass renderPass, vk::Viewport viewport, vk::Rect2D scissor, const Settings* settings );

    /// Initializes a ray tracing pipeline.
    /// @param descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    void init( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, const Settings* settings );

    /// @return Returns the Vulkan pipeline object without the unique handle.
    inline vk::Pipeline get( ) const { return this->pipeline.get( ); }

    /// @return Returns the Vulkan pipeline layout object without the unique handle.
    inline vk::PipelineLayout getLayout( ) const { return this->layout.get( ); }

  private:
    vk::UniquePipeline pipeline;     ///< The Vulkan pipeline with a unique handle.
    vk::UniquePipelineLayout layout; ///< The Vulkan pipeline layout with a unique handle.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // PIPELINE_HPP
