#ifndef DEPTH_IMAGE_HPP
#define DEPTH_IMAGE_HPP

#include "Image.hpp"
#include "ImageView.hpp"

namespace RX
{
  class DepthImage : public Image
  {
  public:
    inline VkImage get() const { return m_image; }
    inline VkImageView getView() const { return m_imageView.get(); }

    void initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D swapchainExtent);
    void transitionToLayout(VkImageLayout layout) override;

    void destroy();

    static VkFormat getSupportedFormat(VkPhysicalDevice physicalDevice);

  private:
    ImageView m_imageView;
  };
}

#endif // DEPTH_IMAGE_HPP