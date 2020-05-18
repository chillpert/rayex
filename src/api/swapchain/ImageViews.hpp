#ifndef IMAGE_VIEWS_HPP
#define IMAGE_VIEWS_HPP

#include "api/BaseComponent.hpp"
#include "api/Surface.hpp"
#include "Images.hpp"

namespace RX
{
	class ImageViews : public BaseComponent
	{
	public:
		ImageViews();
		~ImageViews();

		inline std::vector<VkImageView>& get() { return m_imageViews; }

		void initialize(VkDevice device, VkFormat surfaceFormat, Images& images);
	  void destroy();

	private:
		std::vector<VkImageView> m_imageViews;
		VkDevice m_device;
	};
}

#endif // IMAGE_VIEWS_HPP