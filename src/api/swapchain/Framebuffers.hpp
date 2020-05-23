#ifndef FRAMEBUFFERS_HPP
#define FRAMEBUFFERS_HPP

#include "window/Window.hpp"
#include "api/BaseComponent.hpp"
#include "api/RenderPass.hpp"
#include "ImageViews.hpp"

namespace RX
{
	class Framebuffers : public BaseComponent
	{
	public:
    Framebuffers();
    ~Framebuffers();

		inline std::vector<VkFramebuffer> get() { return m_framebuffers; }
		inline size_t getSize() { return m_framebuffers.size(); }

		void initialize(VkDevice device, ImageViews& imageViews, VkRenderPass renderPass, std::shared_ptr<Window> window);
    void destroy();

	private:
		std::vector<VkFramebuffer> m_framebuffers;
    VkDevice m_device;
	};
}

#endif // FRAMEBUFFERS_HPP