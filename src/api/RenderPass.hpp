#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct RenderPassInfo
  {
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    std::vector<vk::AttachmentDescription> attachments;
    std::vector<vk::SubpassDescription> subpasses;
    std::vector<vk::SubpassDependency> dependencies;
  };

  /*
  struct RenderPassInfo
  {
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::Format surfaceFormat;
    vk::Format depthFormat;
    vk::ImageLayout initialLayout = vk::ImageLayout::eUndefined;
    vk::ImageLayout finalLayout = vk::ImageLayout::ePresentSrcKHR;
    bool guiEnabled;
  };
  */

  struct RenderPassBeginInfo
  {
    vk::Rect2D renderArea;
    std::vector<vk::ClearValue> clearValues;
    std::vector<vk::CommandBuffer> commandBuffers;
    std::vector<vk::Framebuffer> framebuffers;
  };

  class RenderPass
  {
  public:
    RenderPass() = default;
    RenderPass(RenderPassInfo& info);
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
    vk::RenderPass m_renderPass;
    RenderPassInfo m_info;
    RenderPassBeginInfo m_beginInfo;
  };
}

#endif // RENDER_PASS_HPP
