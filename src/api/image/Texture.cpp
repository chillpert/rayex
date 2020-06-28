#include "Texture.hpp"
#include "api/buffers/Buffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace RX
{
  Texture::Texture(TextureInfo& info)
  {
    init(info);
  }

  Texture::Texture(TextureInfo&& info)
  {
    init(info);
  }

  Texture::~Texture()
  {
    destroy();
  }

  void Texture::init(TextureInfo& info)
  {
    m_info = info;

    int width, height, channels;
    stbi_uc* pixels = stbi_load(m_info.path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels)
      RX_ERROR("Failed to load texture");

    // Set up the staging buffer.
    BufferInfo stagingInfo{ };
    stagingInfo.physicalDevice = m_info.physicalDevice;
    stagingInfo.device = m_info.device;
    stagingInfo.size = width * height * 4;
    stagingInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
    stagingInfo.sharingMode = vk::SharingMode::eConcurrent;
    stagingInfo.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    stagingInfo.queue = m_info.queue;
    stagingInfo.commandPool = m_info.commandPool;
    stagingInfo.queueFamilyIndices = m_info.queueIndices;

    Buffer stagingBuffer(stagingInfo);
    stagingBuffer.fill<stbi_uc>(pixels);

    stbi_image_free(pixels);

    ImageInfo imageInfo{ };
    imageInfo.physicalDevice = m_info.physicalDevice;
    imageInfo.device = m_info.device;
    imageInfo.queue = m_info.queue;
    imageInfo.commandPool = m_info.commandPool;
    imageInfo.extent = vk::Extent3D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

    m_image.init(imageInfo);

    m_image.transitionToLayout(vk::ImageLayout::eTransferDstOptimal);
    stagingBuffer.copyToImage(m_image);
    m_image.transitionToLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

    m_imageView.init({ m_info.device, m_image.get(), m_image.getFormat() });
    m_sampler.init();
  }

  void Texture::init(TextureInfo&& info)
  {
    init(info);
  }

  void Texture::destroy()
  {

  }
}