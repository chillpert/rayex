#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "WindowBase.hpp"

namespace RX
{
	struct FramebufferInfo
	{
		vk::Device device;
		vk::ImageView imageView; // TODO: make this an array, so that depth image view doesn't need to be passed separetly anymore.
		vk::ImageView depthImageView = nullptr;
		vk::RenderPass renderPass;
		vk::Extent2D extent;
	};

	class Framebuffer
	{
	public:
    ~Framebuffer();

		inline vk::Framebuffer get() { return m_framebuffer; }

		void initialize(FramebufferInfo& info);
    void destroy();

	private:
		vk::Framebuffer m_framebuffer;
		FramebufferInfo m_info;
	};
}

#endif // FRAMEBUFFER_HPP