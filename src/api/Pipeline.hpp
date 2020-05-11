#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "window/Window.hpp"
#include "Shader.hpp"

namespace RX
{
  class Pipeline
  {
  public:
    inline VkPipeline get() { return pipeline; }

    void create(VkDevice device, VkRenderPass renderPass, std::shared_ptr<Window> window, Shader& vs, Shader& fs);
    void destroy(VkDevice device);

  private:
    VkPipeline pipeline;
  };
}

#endif // PIPELINE_HPP