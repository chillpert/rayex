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
    RenderPass(RenderPassInfo&& info);
    ~RenderPass();

    inline vk::RenderPass get() { return m_renderPass; }
    inline RenderPassBeginInfo& getBeginInfo() { return m_beginInfo; }

    void init(RenderPassInfo& info);
    void init(RenderPassInfo&& info);

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
