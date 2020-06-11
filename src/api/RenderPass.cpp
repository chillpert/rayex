#include "RenderPass.hpp"

namespace RX
{
  RenderPass::~RenderPass()
  {
    if (m_renderPass)
      destroy();
  }

  void RenderPass::initialize(RenderPassInfo& info)
  {
    m_info = info;

    vk::AttachmentDescription colorAttachmentDescription;
    colorAttachmentDescription.format = info.surfaceFormat;
    colorAttachmentDescription.samples = vk::SampleCountFlagBits::e1;
    colorAttachmentDescription.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachmentDescription.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachmentDescription.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachmentDescription.initialLayout = m_info.initialLayout;

    if (m_info.guiEnabled)
      colorAttachmentDescription.finalLayout = vk::ImageLayout::eColorAttachmentOptimal; 
    else
      colorAttachmentDescription.finalLayout = m_info.finalLayout;

    vk::AttachmentReference colorAttachmentReference;
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::AttachmentDescription depthAttachmentDescription;
    depthAttachmentDescription.format = m_info.depthFormat;
    depthAttachmentDescription.samples = vk::SampleCountFlagBits::e1;
    depthAttachmentDescription.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachmentDescription.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachmentDescription.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachmentDescription.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachmentDescription.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentReference depthAttachmentRef;
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::SubpassDescription subpassDescription;
    subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentRef;

    vk::SubpassDependency subpassDependency;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    //subpassDependency.srcAccessMask = vk::AccessFlagBits:: ;
    subpassDependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    subpassDependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    
    std::array<vk::AttachmentDescription, 2> attachmentDescriptions{ colorAttachmentDescription, depthAttachmentDescription };
    std::array<vk::SubpassDescription, 1> subpassDescriptions{ subpassDescription };
    std::array<vk::SubpassDependency, 1> subpassDependencies{ subpassDependency };

    vk::RenderPassCreateInfo createInfo;
    createInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
    createInfo.pAttachments = attachmentDescriptions.data();
    createInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
    createInfo.pSubpasses = subpassDescriptions.data();
    createInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
    createInfo.pDependencies = subpassDependencies.data();

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