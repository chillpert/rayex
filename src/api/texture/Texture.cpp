#include "Texture.hpp"
#include "api/buffers/Buffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace RX
{
  void Texture::initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool commandPool, const std::string& path)
  {
    int width, height, channels;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels)
      RX_ERROR("Failed to load texture");

    // Set up the staging buffer.
    BufferCreateInfo stagingInfo{ };
    stagingInfo.physicalDevice = physicalDevice;
    stagingInfo.device = device;
    stagingInfo.deviceSize = width * height * 4;
    stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    stagingInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingInfo.commandPool = commandPool;
    stagingInfo.queue = queue;

    Buffer stagingBuffer;
    stagingBuffer.create(stagingInfo);
    stagingBuffer.fill<stbi_uc>(pixels);

    stbi_image_free(pixels);

    ImageInfo imageInfo{ };
    imageInfo.physicalDevice = physicalDevice;
    imageInfo.device = device;
    imageInfo.queue = queue;
    imageInfo.commandPool = commandPool;
    imageInfo.extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

    m_image.initialize(imageInfo);

    m_image.transitionToLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    stagingBuffer.copyToImage(m_image);
    m_image.transitionToLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    ImageViewInfo imageViewInfo{ };
    imageViewInfo.device = device;
    imageViewInfo.format = m_image.getInfo().format;
    imageViewInfo.image = m_image.get();
    m_imageView.initialize(imageViewInfo);

    m_sampler.initialize(device);
  }
}