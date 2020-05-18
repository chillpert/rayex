#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "BaseComponent.hpp"

namespace RX
{
  class RenderPass : public BaseComponent
  {
  public:
    RenderPass();
    ~RenderPass();

    inline VkRenderPass get() { return m_renderPass; }

    void initialize(VkDevice device, VkFormat format);
    void destroy();

  private:
    VkRenderPass m_renderPass;
    VkDevice m_device;
  };
}

#endif // RENDER_PASS_HPP
