#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class RenderPass
  {
  public:
    inline VkRenderPass get() { return renderPass; }

    void create(VkDevice device, VkFormat format);
    void destroy(VkDevice device);

  private:
    VkRenderPass renderPass;
  };
}

#endif // RENDER_PASS_HPP
