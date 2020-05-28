#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Image.hpp"
#include "ImageView.hpp"
#include "Sampler.hpp"

namespace RX
{
  class Texture
  {
  public:
    inline VkImage getImage() { return m_image.get(); }
    inline VkImageView getImageView() { return m_imageView.get(); }
    inline VkSampler getSampler() { return m_sampler.get(); }

    void initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool commandPool, const std::string& path);
  
  private:
    Image m_image;
    ImageView m_imageView;
    Sampler m_sampler;
  };
}

#endif // TEXTURE_HPP