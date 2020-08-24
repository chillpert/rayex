#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "WindowBase.hpp"
#include "Swapchain.hpp"

namespace rx
{
  class Pipeline
  {
  public:
    Pipeline( ) = default;
    Pipeline( vk::Viewport viewport,
              vk::Rect2D scissor,
              const std::vector<vk::DescriptorSetLayout> & descriptorSetLayouts,
              vk::ShaderModule rayGen,
              vk::ShaderModule miss,
              vk::ShaderModule closestHit,
              uint32_t maxRecursion = 4,
              bool initialize = true );

    void init( vk::Viewport viewport,
               vk::Rect2D scissor,
               const std::vector<vk::DescriptorSetLayout> & descriptorSetLayouts,
               vk::ShaderModule rayGen,
               vk::ShaderModule miss,
               vk::ShaderModule closestHit,
               uint32_t maxRecursion = 4 );

    inline const vk::Pipeline get( ) const { return m_pipeline.get( ); }
    inline const vk::PipelineLayout getLayout( ) const { return m_layout.get( ); }
    inline const vk::Rect2D getScissor( ) const { return m_scissor; }
    inline const vk::Viewport getViewport( ) const { return m_viewport; }

    void bind( vk::CommandBuffer commandBuffer ) const;

  private:
    vk::UniquePipeline m_pipeline;
    vk::UniquePipelineLayout m_layout;
    vk::Viewport m_viewport;
    vk::Rect2D m_scissor;
  };
}

#endif // PIPELINE_HPP