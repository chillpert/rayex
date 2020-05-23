#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "BaseComponent.hpp"
#include "window/Window.hpp"
#include "swapchain/Swapchain.hpp"
#include "Shader.hpp"

namespace RX
{
  class Pipeline : public BaseComponent
  {
  public:
    Pipeline();
    ~Pipeline();

    inline VkPipeline get() { return m_pipeline; }

    void initialize(VkDevice device, VkRenderPass renderPass, VkExtent2D& extent, std::shared_ptr<Window> window, Shader& vs, Shader& fs, VkDescriptorSetLayout descriptorSetLayout);
    void destroy();

  private:
    VkPipeline m_pipeline;
    VkPipelineLayout m_layout;
    VkDevice m_device;
  };
}

#endif // PIPELINE_HPP