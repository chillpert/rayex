#ifndef IMAGES_HPP
#define IMAGES_HPP

#include "pch/stdafx.hpp"

namespace RX
{
	class Images
	{
	public:
		inline std::vector<VkImage>& get() { return m_images; }

		void initialize(VkDevice device, VkSwapchainKHR swapchain)
	  {
	    uint32_t imageCount;
	    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr), "Failed to get swap chain images");

	    m_images.resize(imageCount);
	    VK_ASSERT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, m_images.data()), "Failed to get swap chain images");
	  }

	private:
		std::vector<VkImage> m_images;
	};
}

#endif // IMAGES_HPP