#include "Framebuffer.hpp"

namespace RX
{
  Framebuffer::Framebuffer(FramebufferInfo& info)
  {
    init(info);
  }

  Framebuffer::Framebuffer(FramebufferInfo&& info)
  {
    init(info);
  }

  Framebuffer::~Framebuffer()
  {
    if (m_framebuffer)
      destroy();
  }

  void Framebuffer::init(FramebufferInfo& info)
  {
    m_info = info;

    std::vector<vk::ImageView> attachments{ m_info.imageView };

    // Note: It seems like ==operator is bugged, since != nullptr does not work directly
    void* temp = m_info.depthImageView;

    if (temp != nullptr)
      attachments.push_back(m_info.depthImageView);

    vk::FramebufferCreateInfo createInfo;
    createInfo.renderPass = m_info.renderPass;
    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();
    createInfo.width = m_info.extent.width;
    createInfo.height = m_info.extent.height;
    createInfo.layers = 1;

    m_framebuffer = m_info.device.createFramebuffer(createInfo);
    if (!m_framebuffer)
      RX_ERROR("Failed to create framebuffer.");
  }

  void Framebuffer::init(FramebufferInfo&& info)
  {
    init(info);
  }

  void Framebuffer::destroy()
  {
    m_info.device.destroyFramebuffer(m_framebuffer);
    m_framebuffer = nullptr;
  }
}