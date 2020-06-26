#include "ImageView.hpp"

namespace RX
{
  ImageView::ImageView(ImageViewInfo& info)
  {
    init(info);
  }

  ImageView::ImageView(ImageViewInfo&& info)
  {
    init(info);
  }

  ImageView::~ImageView()
  {
    if (m_imageView)
      destroy();
  }

  void ImageView::init(ImageViewInfo& info)
  {
    m_info = info;

    vk::ImageViewCreateInfo createInfo;
    createInfo.image = m_info.image;
    createInfo.viewType = m_info.viewType;
    createInfo.format = m_info.format;
    createInfo.components = m_info.components;
    createInfo.subresourceRange = m_info.subresourceRange;

    m_imageView = m_info.device.createImageView(createInfo);

    if (!m_imageView)
      RX_ERROR("Failed to create image view.");
  }

  void ImageView::init(ImageViewInfo&& info)
  {
    init(info);
  }

  void ImageView::destroy()
  {
    m_info.device.destroyImageView(m_imageView);
  }
}