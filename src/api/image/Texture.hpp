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
    RX_API Texture() = default;
    RX_API Texture(TextureInfo& info);
    RX_API ~Texture();

    inline vk::Image getImage() { return m_image.get(); }
    inline vk::ImageView getImageView() { return m_imageView.get(); }
    inline vk::Sampler getSampler() { return m_sampler.get(); }
    inline const std::string& getPath() const { return m_info.path; }

    void initialize(TextureInfo& info);
    void destroy();
  
  private:
    Image m_image;
    ImageView m_imageView;
    Sampler m_sampler;
    TextureInfo m_info;
  };
}

namespace std
{
  template<> struct hash<RX::Texture>
  {
    size_t operator()(const std::shared_ptr<RX::Texture> texture) const { return hash<std::string>()(texture->getPath()); }
  };
}

#endif // TEXTURE_HPP