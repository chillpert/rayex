#include "Texture.hpp"
#include "api/buffers/Buffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace RX
{
  void Texture::initialize(TextureInfo& info)
  {
    m_info = info;

    int width, height, channels;
    stbi_uc* pixels = stbi_load(m_info.path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels)
      RX_ERROR("Failed to load texture");

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = m_info.physicalDevice;
    stagingInfo.device = m_info.device;
    stagingInfo.deviceSize = width * height * 4;
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingInfo.commandPool = m_info.commandPool;
    stagingInfo.queue = m_info.queue;

    Buffer stagingBuffer;
    stagingBuffer.initialize(stagingInfo);
    stagingBuffer.fill<stbi_uc>(pixels);

    stbi_image_free(pixels);

    ImageInfo imageInfo{ };
    imageInfo.physicalDevice = m_info.physicalDevice;
    imageInfo.device = m_info.device;
    imageInfo.queue = m_info.queue;
    imageInfo.commandPool = m_info.commandPool;
    imageInfo.extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

    m_image.initialize(imageInfo);

    m_image.transitionToLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    stagingBuffer.copyToImage(m_image);
    m_image.transitionToLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    ImageViewInfo imageViewInfo{ };
    imageViewInfo.device = m_info.device;
    imageViewInfo.format = m_image.getInfo().format;
    imageViewInfo.image = m_image.get();
    m_imageView.initialize(imageViewInfo);

    m_sampler.initialize(m_info.device);
  }
}