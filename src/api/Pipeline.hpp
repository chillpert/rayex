#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "api/Shader.hpp"
#include "api/SwapChain.hpp"

namespace RX
{
  class Pipeline
  {
  public:
    Pipeline(VkDevice* logicalDevice, SwapChain* swapChain);

    void createRenderPass();
    void createGraphicsPipeline();

    void destroyRenderPass();
    void destroyGraphicsPipelineLayout();
    void destroyGraphicsPipeline();

    inline VkPipeline* getPipeline() { return &m_graphicsPipeline; }
    inline VkRenderPass* getRenderPass() { return &m_renderPass; }

  private:
    VkPipeline m_graphicsPipeline;
    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;

    VkDevice* m_logicalDevice;

    // A pointer to VulkanApi class members
    SwapChain* m_swapChain;

    std::shared_ptr<Shader> m_vertexShader;
    std::shared_ptr<Shader> m_fragmentShader;
  };
}

#endif // GRAPHICS_HPP