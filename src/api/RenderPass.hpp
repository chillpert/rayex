#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct RenderPassInfo
  {
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkFormat surfaceFormat; // Should be the same as the surface format.
    VkFormat depthFormat;
    VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  };

  class RenderPass
  {
  public:
    ~RenderPass();

    inline VkRenderPass get() { return m_renderPass; }
    inline RenderPassInfo getInfo() { return m_info; }

    void initialize(RenderPassInfo& info);

    void destroy();
  private:
    
    VkRenderPass m_renderPass;
    RenderPassInfo m_info;

    bool m_created = false;
  };
}

#endif // RENDER_PASS_HPP
