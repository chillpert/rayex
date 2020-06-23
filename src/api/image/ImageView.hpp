#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct ImageViewInfo
  {
    vk::Device device;
    vk::Image image;
    vk::Format format;
    vk::ImageViewType viewType = vk::ImageViewType::e2D;
    vk::ComponentMapping components = { vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity };
    vk::ImageSubresourceRange subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };
  };

  class ImageView
  {
  public:
    ImageView() = default;
    ImageView(ImageViewInfo& info);
    ~ImageView();

    inline vk::ImageView get() { return m_imageView; }

    void initialize(ImageViewInfo& info);
    void destroy();

  private:
    vk::ImageView m_imageView;
    ImageViewInfo m_info;
  };
}

#endif // IMAGE_VIEW_HPP