#include "Framebuffer.hpp"

namespace RX
{
  Framebuffer::~Framebuffer()
  {
    destroy();
  }

  void Framebuffer::initialize(FramebufferInfo& info)
  {
    m_info = info;

    std::vector<VkImageView> attachments{ m_info.imageView };

    if (m_info.depthImageView != VK_NULL_HANDLE)
      attachments.push_back(m_info.depthImageView);

    VkFramebufferCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = m_info.renderPass;
    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();
    createInfo.width = m_info.extent.width;
    createInfo.height = m_info.extent.height;
    createInfo.layers = 1;

    VK_ASSERT(vkCreateFramebuffer(m_info.device, &createInfo, nullptr, &m_framebuffer), "Failed to create frame buffer");
  }

  void Framebuffer::destroy()
  {
    RX_DESTROY(vkDestroyFramebuffer(m_info.device, m_framebuffer, nullptr), "framebuffer");
  }
}