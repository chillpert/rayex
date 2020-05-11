#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "window/Window.hpp"
#include "VkShader.hpp"

namespace RX
{
  class Pipeline
  {
  public:
    inline VkPipeline get() { return pipeline; }

    void create(VkDevice device, VkRenderPass renderPass, std::shared_ptr<Window> window, std::shared_ptr<VkShader> vs, std::shared_ptr<VkShader> fs);
    void destroy(VkDevice device);

  private:
    VkPipeline pipeline;
  };
}

#endif // PIPELINE_HPP