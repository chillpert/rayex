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

  struct RenderPassBeginInfo
  {
    VkRect2D renderArea;
    std::vector<VkClearValue> clearValues;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkFramebuffer> framebuffers;
  };

  class RenderPass
  {
  public:
    ~RenderPass();

    inline VkRenderPass get() { return m_renderPass; }
    inline RenderPassInfo& getInfo() { return m_info; }
    inline RenderPassBeginInfo& getBeginInfo() { return m_beginInfo; }

    void initialize(RenderPassInfo& info);
    void destroy();

    void setBeginInfo(RenderPassBeginInfo& beginInfo);
    void begin(size_t index = 0);
    void end(size_t index = 0);

  private:
    
    VkRenderPass m_renderPass;
    RenderPassInfo m_info;
    RenderPassBeginInfo m_beginInfo;

    bool m_created = false;
  };
}

#endif // RENDER_PASS_HPP
