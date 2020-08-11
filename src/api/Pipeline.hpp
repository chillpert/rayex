#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "WindowBase.hpp"
#include "Swapchain.hpp"

namespace rx
{
  class Pipeline
  {
  public:
    virtual ~Pipeline( ) = default;

    inline const vk::Pipeline get( ) const { return m_pipeline.get( ); }
    inline const vk::PipelineLayout getLayout( ) const { return m_layout.get( ); }
    inline const vk::Rect2D getScissor( ) const { return m_scissor; }
    inline const vk::Viewport getViewport( ) const { return m_viewport; }

    virtual void bind( vk::CommandBuffer commandBuffer ) const = 0;

  protected:
    vk::UniquePipeline m_pipeline;
    vk::UniquePipelineLayout m_layout;
    vk::Viewport m_viewport;
    vk::Rect2D m_scissor;
  };

  class RasterizationPipeline : public Pipeline
  {
  public:
    RasterizationPipeline( ) = default;
    RasterizationPipeline( vk::RenderPass renderPass,
                           vk::Viewport viewport,
                           vk::Rect2D scissor,
                           vk::ShaderModule vertexShader,
                           vk::ShaderModule fragmentShader,
                           vk::DescriptorSetLayout descriptorSetLayout,
                           vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList,
                           bool initialize = true );

    void init( vk::RenderPass renderPass,
               vk::Viewport viewport,
               vk::Rect2D scissor,
               vk::ShaderModule vertexShader,
               vk::ShaderModule fragmentShader,
               vk::DescriptorSetLayout descriptorSetLayout,
               vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList );

    void bind( vk::CommandBuffer commandBuffer ) const;
  };

  class RayTracingPipeline : public Pipeline
  {
  public:
    RayTracingPipeline( ) = default;
    RayTracingPipeline( vk::RenderPass renderPass,
                        vk::Viewport viewport,
                        vk::Rect2D scissor,
                        const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                        vk::ShaderModule rayGen,
                        vk::ShaderModule miss,
                        vk::ShaderModule closestHit,
                        uint32_t maxRecursion = 4,
                        bool initialize = true );

    void init( vk::RenderPass renderPass,
               vk::Viewport viewport,
               vk::Rect2D scissor,
               const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
               vk::ShaderModule rayGen,
               vk::ShaderModule miss,
               vk::ShaderModule closestHit,
               uint32_t maxRecursion = 4 );

    void bind( vk::CommandBuffer commandBuffer ) const;
  };
}

#endif // PIPELINE_HPP