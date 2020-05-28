#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Window.hpp"
#include "Swapchain.hpp"
#include "Shader.hpp"

namespace RX
{
  struct PipelineInfo
  {
    VkDevice device;
    VkRenderPass renderPass;
    VkViewport viewport;
    VkRect2D scissor;
    VkShaderModule vertexShader;
    VkShaderModule fragmentShader;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  };

  class Pipeline
  {
  public:
    ~Pipeline();

    inline VkPipeline get() { return m_pipeline; }
    inline VkPipelineLayout getLayout() { return m_layout; }
    inline PipelineInfo getInfo() { return m_info; }

    void initialize(PipelineInfo& info);
    void destroy();

  private:
    VkPipeline m_pipeline;
    VkPipelineLayout m_layout;
    PipelineInfo m_info;

    bool m_created = false;
  };
}

#endif // PIPELINE_HPP