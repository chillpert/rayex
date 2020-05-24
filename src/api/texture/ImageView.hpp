#ifndef IMAGE_VIEW_HPP
#define IMAGE_VIEW_HPP

#include "Image.hpp"

namespace RX
{
  class ImageView : public BaseComponent
  {
  public:
    ImageView();
    ~ImageView();

    inline VkImageView get() const { return m_imageView; }

    void initialize(Image& image);
    void destroy();

  private:
    VkImageView m_imageView;
    VkDevice m_device;
  };
}

#endif // IMAGE_VIEW_HPP