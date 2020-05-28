#include "ImageView.hpp"

namespace RX
{
  ImageView::~ImageView()
  {
    destroy();
  }

  void ImageView::initialize(ImageViewInfo& info)
  {
    m_info = info;

    VkImageViewCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = m_info.image;
    createInfo.viewType = m_info.viewType;
    createInfo.format = m_info.format;
    createInfo.components = m_info.components;
    createInfo.subresourceRange = m_info.subresourceRange;

    VK_ASSERT(vkCreateImageView(m_info.device, &createInfo, nullptr, &m_imageView), "Failed to create image view");
  }

  void ImageView::destroy()
  {
    VK_DESTROY(vkDestroyImageView(m_info.device, m_imageView, nullptr), "image view");
  }
}