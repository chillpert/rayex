#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "window/Window.hpp"
#include "swapchain/Swapchain.hpp"
#include "Shader.hpp"

namespace RX
{
  class Pipeline
  {
  public:
    ~Pipeline();

    inline VkPipeline get() { return m_pipeline; }
    inline VkPipelineLayout getLayout() { return m_layout; }

    void initialize(VkDevice device, VkRenderPass renderPass, VkExtent2D& extent, std::shared_ptr<Window> window, Shader& vs, Shader& fs, VkDescriptorSetLayout descriptorSetLayout);
    void destroy();

  private:
    VkPipeline m_pipeline;
    VkPipelineLayout m_layout;
    VkDevice m_device;

    bool m_created = false;
  };
}

#endif // PIPELINE_HPP