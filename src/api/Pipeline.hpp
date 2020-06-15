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
    vk::ShaderModule vertexShader;
    vk::ShaderModule fragmentShader;
    vk::DescriptorSetLayout descriptorSetLayout;
    vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList;
  };

  class Pipeline
  {
  public:
    ~Pipeline();

    inline vk::Pipeline get() { return m_pipeline; }
    inline vk::PipelineLayout getLayout() { return m_layout; }
    inline PipelineInfo& getInfo() { return m_info; }

    void initialize(PipelineInfo& info);
    void destroy();

  private:
    vk::Pipeline m_pipeline;
    vk::PipelineLayout m_layout;
    PipelineInfo m_info;
  };
}

#endif // PIPELINE_HPP