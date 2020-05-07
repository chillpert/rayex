#ifndef GRAPHICS_PIPELINE_HPP
#define GRAPHICS_PIPELINE_HPP

#include "api/Shader.hpp"
#include "api/vulkan/SwapChain.hpp"

namespace RX
{
  class GraphicsPipeline
  {
  public:
    GraphicsPipeline(VkDevice* logicalDevice, SwapChain* swapChain);

    void createGraphicsPipeline();
    void destroyGraphicsPipeline();
    
  private:
    VkPipelineLayout m_pipelineLayout;

    VkDevice* m_logicalDevice;

    // pointer to VulkanApi class members
    SwapChain* m_swapChain;

    std::shared_ptr<Shader> m_vertexShader;
    std::shared_ptr<Shader> m_fragmentShader;
  };
}

#endif // GRAPHICS_PIPELINE_HPP