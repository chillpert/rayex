#ifndef IMAGES_HPP
#define IMAGES_HPP

#include "pch/stdafx.hpp"

namespace RX
{
	class Images
	{
	public:
		inline std::vector<VkImage>& get() { return m_images; }

		void initialize(VkDevice device, VkSwapchainKHR swapchain);

	private:
		std::vector<VkImage> m_images;
	};
}

#endif // IMAGES_HPP