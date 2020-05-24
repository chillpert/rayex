#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "Image.hpp"

namespace RX
{
  class ImageView
  {
  public:
    void initialize(Image& image);

  private:
    VkImageView m_imageView;
  };
}

#endif // IMAGE_VIEW_HPP