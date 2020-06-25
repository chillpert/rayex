#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "WindowBase.hpp"
#include "Swapchain.hpp"
#include "Shader.hpp"

namespace RX
{
  struct PipelineInfo
  {
    vk::Device device;
    vk::RenderPass renderPass;
    vk::Viewport viewport;
    vk::Rect2D scissor;
  };

  struct RasterizationPipelineInfo : public PipelineInfo
  {
    vk::ShaderModule vertexShader;
    vk::ShaderModule fragmentShader;
    vk::DescriptorSetLayout descriptorSetLayout;
    vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList;
  };

  struct RaytracingPipelineInfo : public PipelineInfo
  {
    uint32_t maxRecursion = 4;
    std::vector<vk::DescriptorSetLayout> layouts;
    Shader* rayGen;
    Shader* miss;
    Shader* closestHit;
  };

  class Pipeline
  {
  public:
    ~Pipeline();

    inline vk::Pipeline get() { return m_pipeline; }
    inline vk::PipelineLayout getLayout() { return m_layout; }
    inline vk::Rect2D getScissor() const { return m_info.scissor; }
    inline vk::Viewport getViewport() const { return m_info.viewport; }

    void initialize(RasterizationPipelineInfo& info);
    void initialize(RaytracingPipelineInfo& info);
    void destroy();

  private:
    vk::Pipeline m_pipeline;
    vk::PipelineLayout m_layout;

    PipelineInfo m_info;
  };
}

#endif // PIPELINE_HPP