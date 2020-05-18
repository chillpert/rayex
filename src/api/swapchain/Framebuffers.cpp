#include "Framebuffers.hpp"

namespace RX
{
  Framebuffers::Framebuffers() :
    BaseComponent("Framebuffers") { }

  Framebuffers::~Framebuffers()
  {
    destroy();
  }

  void Framebuffers::initialize(VkDevice device, ImageViews& imageViews, VkRenderPass renderPass, std::shared_ptr<Window> window)
  {
    m_device = device;

    m_framebuffers.resize(imageViews.get().size());

    uint32_t imageCount = static_cast<uint32_t>(imageViews.get().size());

    int width, height;
    window->getSize(&width, &height);

    for (uint32_t i = 0; i < imageCount; ++i)
    {
      VkFramebufferCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      createInfo.renderPass = renderPass;
      createInfo.attachmentCount = 1;
      createInfo.pAttachments = &imageViews.get()[i];
      createInfo.width = static_cast<uint32_t>(width);
      createInfo.height = static_cast<uint32_t>(height);

      createInfo.layers = 1;

      VK_ASSERT(vkCreateFramebuffer(device, &createInfo, nullptr, &m_framebuffers[i]), "Failed to create frame buffer");
    }

    initializationCallback();
  }

  void Framebuffers::destroy()
  {
    assertDestruction();

    for (auto framebuffer : m_framebuffers)
      vkDestroyFramebuffer(m_device, framebuffer, nullptr);
  }
}