#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "window/Window.hpp"

namespace RX
{
	struct FramebufferInfo
	{
		VkDevice device;
		VkImageView imageView; // TODO: make this an array, so that depth image view doesn't need to be passed separetly anymore.
		VkImageView depthImageView = VK_NULL_HANDLE;
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