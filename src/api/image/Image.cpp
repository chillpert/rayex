#include "Texture.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/CommandBuffer.hpp"

namespace RX
{
  Image::~Image()
  {
    destroy();
  }
  
  void Image::initialize(ImageInfo& info)
  {
    m_info = info;

    VkImageCreateInfo createInfo{ };
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType = m_info.imageType;
    createInfo.format = m_info.format;
    createInfo.extent = m_info.extent;
    createInfo.mipLevels = m_info.mipLevels;
    createInfo.arrayLayers = m_info.arrayLayers;
    createInfo.samples = m_info.samples;
    createInfo.tiling = m_info.tiling;
    createInfo.usage = m_info.usage;
    createInfo.sharingMode = m_info.sharingMode;
    createInfo.initialLayout = m_info.layout;
    
    VK_CREATE(vkCreateImage(m_info.device, &createInfo, nullptr, &m_image), "image");

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(m_info.device, m_image, &memoryRequirements);

    VkMemoryAllocateInfo allocateInfo{ };
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = Buffer::findMemoryType(m_info.physicalDevice, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_ASSERT(vkAllocateMemory(m_info.device, &allocateInfo, nullptr, &m_memory), "Failed to allocate memory for image.");
    VK_ASSERT(vkBindImageMemory(m_info.device, m_image, m_memory, 0), "Failed to bind image memory.");
  }

  void Image::transitionToLayout(VkImageLayout layout)
  {
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = m_info.commandPool;
    commandBufferInfo.queue = m_info.queue;

    CommandBuffer commandBuffer;
    commandBuffer.initialize(commandBufferInfo);

    commandBuffer.begin();

    VkImageMemoryBarrier barrier{ };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = m_info.layout;
    barrier.newLayout = layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = m_image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (m_info.layout == VK_IMAGE_LAYOUT_UNDEFINED && layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (m_info.layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
      RX_ERROR("Failed to transition image layout.");

    vkCmdPipelineBarrier
    (
      commandBuffer.getFront(),
      sourceStage, destinationStage,
      0,
      0, nullptr,
      0, nullptr,
      1, &barrier
    );

    commandBuffer.end();

    m_info.layout = layout;
  }

  VkFormat Image::findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& formatsToTest, VkFormatFeatureFlags features, VkImageTiling tiling)
  {
    for (VkFormat format : formatsToTest)
    {
      VkFormatProperties props;
      vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

      if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        return format;

      else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        return format;
    }

    RX_ERROR("Failed to retrieve any supported image format.");

    return VK_FORMAT_UNDEFINED;
  }

  void Image::destroy()
  {
    VK_DESTROY(vkDestroyImage(m_info.device, m_image, nullptr), "image");
    VK_FREE(vkFreeMemory(m_info.device, m_memory, nullptr), "image");

    m_image = VK_NULL_HANDLE;
    m_memory = VK_NULL_HANDLE;
  }
}