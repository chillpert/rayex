#include "RenderPass.hpp"

namespace RX
{
  RenderPass::RenderPass(RenderPassInfo& info)
  {
    initialize(info);
  }

  RenderPass::~RenderPass()
  {
    if (m_renderPass)
      destroy();
  }

  void RenderPass::initialize(RenderPassInfo& info)
  {
    m_info = info;

    vk::RenderPassCreateInfo createInfo;
    createInfo.attachmentCount = static_cast<uint32_t>(m_info.attachments.size());
    createInfo.pAttachments = m_info.attachments.data();
    createInfo.subpassCount = static_cast<uint32_t>(m_info.subpasses.size());
    createInfo.pSubpasses = m_info.subpasses.data();
    createInfo.dependencyCount = static_cast<uint32_t>(m_info.dependencies.size());
    createInfo.pDependencies = m_info.dependencies.data();

    m_renderPass = m_info.device.createRenderPass(createInfo);
  }

  void RenderPass::destroy()
  {
    m_info.device.destroyRenderPass(m_renderPass);
    m_renderPass = nullptr;
  }

  void RenderPass::setBeginInfo(RenderPassBeginInfo& beginInfo)
  {
    m_beginInfo = beginInfo;
  }

  void RenderPass::begin(size_t index)
  {
    vk::RenderPassBeginInfo renderPassInfo;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_beginInfo.framebuffers[index];
    renderPassInfo.renderArea = m_beginInfo.renderArea;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(m_beginInfo.clearValues.size());

    m_beginInfo.clearValues[0].color.float32;

    renderPassInfo.pClearValues = m_beginInfo.clearValues.data();

    m_beginInfo.commandBuffers[index].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline); // CMD
  }

  void RenderPass::end(size_t index)
  {
    m_beginInfo.commandBuffers[index].endRenderPass(); // CMD
  }
}