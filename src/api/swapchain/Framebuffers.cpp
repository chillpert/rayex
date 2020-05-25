#include "Framebuffers.hpp"

namespace RX
{
  Framebuffers::Framebuffers() :
    BaseComponent("Framebuffers") { }

  Framebuffers::~Framebuffers()
  {
    destroy();
  }

  void Framebuffers::initialize(VkDevice device, ImageViews& swapchainImageViews, VkImageView depthImageView, VkRenderPass renderPass, std::shared_ptr<Window> window)
  {
    m_device = device;

    m_framebuffers.resize(swapchainImageViews.get().size());

    uint32_t imageCount = static_cast<uint32_t>(swapchainImageViews.get().size());

    int width, height;
    window->getSize(&width, &height);

    for (uint32_t i = 0; i < imageCount; ++i)
    {
      VkImageView attachments[] = { swapchainImageViews.get()[i], depthImageView };

      VkFramebufferCreateInfo createInfo{ };
      createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      createInfo.renderPass = renderPass;
      createInfo.attachmentCount = 2;
      createInfo.pAttachments = attachments;
      createInfo.width = static_cast<uint32_t>(width); // TODO: use swap chain extent instead
      createInfo.height = static_cast<uint32_t>(height); // TODO: use swap chain extent instead
      createInfo.layers = 1;

      VK_ASSERT(vkCreateFramebuffer(device, &createInfo, nullptr, &m_framebuffers[i]), "Failed to create frame buffer");
    }

    RX_INITIALIZATION_CALLBACK;
  }

  void Framebuffers::destroy()
  {
    RX_ASSERT_DESTRUCTION;

    for (auto framebuffer : m_framebuffers)
      vkDestroyFramebuffer(m_device, framebuffer, nullptr);
  }
}