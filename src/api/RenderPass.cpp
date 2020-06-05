#include "RenderPass.hpp"

namespace RX
{
  RenderPass::~RenderPass()
  {
    destroy();
  }

  void RenderPass::initialize(RenderPassInfo& info)
  {
    m_info = info;

    VkAttachmentDescription colorAttachmentDescription{ };
    colorAttachmentDescription.format = info.surfaceFormat;
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout = m_info.initialLayout;
#ifdef RX_GUI
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
#else
    colorAttachmentDescription.finalLayout = m_info.finalLayout;
#endif

    VkAttachmentReference colorAttachmentReference{ };
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachmentDescription{ };
    depthAttachmentDescription.format = m_info.depthFormat;
    depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{ };
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription{ };
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency subpassDependency{ };
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    
    VkAttachmentDescription attachmentDescriptions[] = { colorAttachmentDescription, depthAttachmentDescription };
    VkRenderPassCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = sizeof(attachmentDescriptions) / sizeof(attachmentDescriptions[0]);
    createInfo.pAttachments = attachmentDescriptions;
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpassDescription;
    createInfo.dependencyCount = 1;
    createInfo.pDependencies = &subpassDependency;

    VK_CREATE(vkCreateRenderPass(m_info.device, &createInfo, nullptr, &m_renderPass), "render pass");
  }

  void RenderPass::destroy()
  {
    VK_DESTROY(vkDestroyRenderPass(m_info.device, m_renderPass, nullptr), "render pass");
    m_renderPass = VK_NULL_HANDLE;
  }

  void RenderPass::setBeginInfo(RenderPassBeginInfo& beginInfo)
  {
    m_beginInfo = beginInfo;
  }

  void RenderPass::begin(size_t index)
  {
    VkRenderPassBeginInfo renderPassInfo{ };
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_beginInfo.framebuffers[index];
    renderPassInfo.renderArea = m_beginInfo.renderArea;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(m_beginInfo.clearValues.size());
    renderPassInfo.pClearValues = m_beginInfo.clearValues.data();

    vkCmdBeginRenderPass(m_beginInfo.commandBuffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  }

  void RenderPass::end(size_t index)
  {
    vkCmdEndRenderPass(m_beginInfo.commandBuffers[index]);
  }
}