#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "BaseComponent.hpp"
#include "window/Window.hpp"
#include "Shader.hpp"

namespace RX
{
  class Pipeline : public BaseComponent
  {
  public:
    Pipeline();

    inline VkPipeline get() { return m_pipeline; }

    void initialize(VkDevice device, VkRenderPass renderPass, std::shared_ptr<Window> window, Shader& vs, Shader& fs);
    void destroy(VkDevice device);

  private:
    VkPipeline m_pipeline;
  };
}

#endif // PIPELINE_HPP