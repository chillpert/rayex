#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "WindowBase.hpp"
#include "Swapchain.hpp"

namespace rx
{
  /// A wrapper class for a Vulkan ray tracing graphics pipeline.
  /// @ingroup API
  class Pipeline
  {
  public:
    Pipeline( ) = default;

    /// @param descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    /// @param initialize If true, the Instance object will be initialized right away without an additional call to init(). 
    Pipeline( const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts, bool initialize = true );

    /// Initializes the pipeline and creates the pipeline layout.
    /// @param descriptorSetLayouts A vector of descriptor set layouts that will be included in the pipeline layout.
    void init( const std::vector<vk::DescriptorSetLayout> & descriptorSetLayouts );

    /// @return Returns the Vulkan pipeline object without the unique handle.
    inline const vk::Pipeline get( ) const { return m_pipeline.get( ); }

    /// @return Returns the Vulkan pipeline layout object without the unique handle.
    inline const vk::PipelineLayout getLayout( ) const { return m_layout.get( ); }

    /// Binds the pipeline for usage.
    /// @param commandBuffer The Vulkan command buffer that will bind the pipeline.
    void bind( vk::CommandBuffer commandBuffer ) const;

  private:
    vk::UniquePipeline m_pipeline; ///< The Vulkan pipeline with a unique handle.
    vk::UniquePipelineLayout m_layout; ///< The Vulkan pipeline layout with a unique handle.
  };
}

#endif // PIPELINE_HPP