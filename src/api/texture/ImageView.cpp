#include "ImageView.hpp"

namespace RX
{
  ImageView::ImageView() :
    BaseComponent("ImageView") { }

  ImageView::~ImageView()
  {
    destroy();
  }

  void ImageView::initialize(Image& image)
  {
    m_device = image.getDevice();

    VkImageViewCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image.get();
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = image.getFormat();
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VK_ASSERT(vkCreateImageView(image.getDevice(), &createInfo, nullptr, &m_imageView), "Failed to create image view");

    RX_INITIALIZATION_CALLBACK;
  }

  void ImageView::destroy()
  {
    RX_ASSERT_DESTRUCTION;
    vkDestroyImageView(m_device, m_imageView, nullptr);
  }
}