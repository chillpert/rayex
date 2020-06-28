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

    if (static_cast<void*>(m_info.depthImageView) != nullptr)
      attachments.push_back(m_info.depthImageView);

    vk::FramebufferCreateInfo createInfo{
      { },                                        // flags
      m_info.renderPass,                          // renderPass
      static_cast<uint32_t>(attachments.size()),  // attachmentCount
      attachments.data(),                         // pAttachments
      m_info.extent.width,                        // width
      m_info.extent.height,                       // height
      1                                           // layers
    };

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