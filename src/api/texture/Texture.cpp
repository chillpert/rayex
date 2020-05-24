#include "Texture.hpp"

namespace RX
{
  void Texture::initialize(VkPhysicalDevice physicalDevice, VkDevice device, const std::string& path)
  {
    int width, height, channels;
    stbi_uc* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

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

    m_buffer.create(bufferInfo);

    // Copy staging buffer to the actual index buffer.
    m_buffer = stagingBuffer;

  }
}