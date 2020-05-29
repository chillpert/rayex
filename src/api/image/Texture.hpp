#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Image.hpp"
#include "ImageView.hpp"
#include "Sampler.hpp"

namespace RX
{
  struct TextureInfo
  {
    std::string path;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue queue;
    VkCommandPool commandPool;
  };

  class Texture
  {
  public:
    inline VkImage getImage() { return m_image.get(); }
    inline VkImageView getImageView() { return m_imageView.get(); }
    inline VkSampler getSampler() { return m_sampler.get(); }
    inline TextureInfo& getInfo() { return m_info; }

    void initialize(TextureInfo& info);
  
  private:
    Image m_image;
    ImageView m_imageView;
    Sampler m_sampler;
    TextureInfo m_info;
  };
}

#endif // TEXTURE_HPP