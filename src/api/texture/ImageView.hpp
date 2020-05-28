#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct ImageViewInfo
  {
    VkDevice device;
    VkImage image;
    VkFormat format;
    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
    VkComponentMapping components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
    VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
  };

  class ImageView
  {
  public:
    RX_API ~ImageView();

    inline VkImageView get() { return m_imageView; }
    inline ImageViewInfo getInfo() { return m_info; }

    void initialize(ImageViewInfo& info);
    void destroy();

  private:
    VkImageView m_imageView;
    ImageViewInfo m_info;
  };
}

#endif // IMAGE_VIEW_HPP