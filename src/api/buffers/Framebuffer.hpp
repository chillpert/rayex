#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "WindowBase.hpp"

namespace RX
{
	struct FramebufferInfo
	{
		vk::Device device;
		vk::RenderPass renderPass;
		vk::Extent2D extent;
		vk::ImageView depthImageView = nullptr;
		vk::ImageView imageView; // TODO: make this an array, so that depth image view doesn't need to be passed separetly anymore.
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		Framebuffer(FramebufferInfo& info);
		Framebuffer(FramebufferInfo&& info);
    ~Framebuffer();

		inline vk::Framebuffer get() { return m_framebuffer; }

		void init(FramebufferInfo& info);
		void init(FramebufferInfo&& info);

    void destroy();

	private:
		vk::Framebuffer m_framebuffer;
		FramebufferInfo m_info;
	};

	void destroy(std::vector<Framebuffer>& framebuffers);
}

#endif // FRAMEBUFFER_HPP