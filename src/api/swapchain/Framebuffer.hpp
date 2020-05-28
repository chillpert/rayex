#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "window/Window.hpp"

namespace RX
{
	struct FramebufferInfo
	{
		std::shared_ptr<Window> window;
		VkDevice device;
		VkImageView imageView;
		VkImageView depthImageView;
		VkRenderPass renderPass;
		VkExtent2D extent;
	};

	class Framebuffer
	{
	public:
    ~Framebuffer();

		inline VkFramebuffer get() { return m_framebuffer; }
		inline FramebufferInfo& getInfo() { return m_info; }

		void initialize(FramebufferInfo& info);
    void destroy();

	private:
		VkFramebuffer m_framebuffer;
		FramebufferInfo m_info;

		bool m_created = false;
	};
}

#endif // FRAMEBUFFER_HPP