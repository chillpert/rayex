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
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::Queue queue;
    vk::CommandPool commandPool;
    std::vector<uint32_t> queueIndices;
  };

  class Texture
  {
  public:
    RX_API ~Texture();

    inline vk::Image getImage() { return m_image.get(); }
    inline vk::ImageView getImageView() { return m_imageView.get(); }
    inline vk::Sampler getSampler() { return m_sampler.get(); }
    inline TextureInfo& getInfo() { return m_info; }

    void initialize(TextureInfo& info);
    void destroy();
  
  private:
    Image m_image;
    ImageView m_imageView;
    Sampler m_sampler;
    TextureInfo m_info;
  };
}

#endif // TEXTURE_HPP