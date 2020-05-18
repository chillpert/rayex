#ifndef FRAMEBUFFERS_HPP
#define FRAMEBUFFERS_HPP

#include "window/Window.hpp"
#include "api/RenderPass.hpp"
#include "ImageViews.hpp"

namespace RX
{
	class Framebuffers
	{
	public:
		inline std::vector<VkFramebuffer> get() { return m_framebuffers; }

		void initialize(VkDevice device, ImageViews& imageViews, VkRenderPass renderPass, std::shared_ptr<Window> window)
	  {
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
	  }

	private:
		std::vector<VkFramebuffer> m_framebuffers;
	};
}

#endif // FRAMEBUFFERS_HPP