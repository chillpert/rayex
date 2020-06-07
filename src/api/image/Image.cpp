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

    vk::ImageCreateInfo createInfo;
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
    
    m_image = m_info.device.createImage(createInfo);

    if (!m_image)
      RX_ERROR("Failed to create image.");

    auto memoryRequirements = m_info.device.getImageMemoryRequirements(m_image);

    vk::MemoryAllocateInfo allocateInfo;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = Buffer::findMemoryType(m_info.physicalDevice, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_memory = m_info.device.allocateMemory(allocateInfo);

    if (!m_memory)
      RX_ERROR("Failed to create memory for image.");

    m_info.device.bindImageMemory(m_image, m_memory, 0);
  }

  void Image::destroy()
  {
    m_info.device.destroyImage(m_image);
    m_info.device.freeMemory(m_memory);
  }

  void Image::transitionToLayout(vk::ImageLayout layout)
  {
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = m_info.commandPool;
    commandBufferInfo.queue = m_info.queue;
    commandBufferInfo.freeAutomatically = true;
    commandBufferInfo.componentName = "command buffer for image layout transitions";

    CommandBuffer commandBuffer;
    commandBuffer.initialize(commandBufferInfo);

    commandBuffer.begin();

    vk::ImageMemoryBarrier barrier;
    barrier.oldLayout = m_info.layout;
    barrier.newLayout = layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = m_image;
    barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vk::PipelineStageFlags sourceStage;
    vk::PipelineStageFlags destinationStage;

    if (m_info.layout == vk::ImageLayout::eUndefined && layout == vk::ImageLayout::eTransferDstOptimal)
    {
      //barrier.srcAccessMask = 0;
      barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

      sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
      destinationStage = vk::PipelineStageFlagBits::eTransfer;
    }
    else if (m_info.layout == vk::ImageLayout::eTransferDstOptimal && layout == vk::ImageLayout::eShaderReadOnlyOptimal)
    {
      barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
      barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

      sourceStage = vk::PipelineStageFlagBits::eTransfer;
      destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
    }
    else
      RX_ERROR("Failed to transition image layout.");

    commandBuffer.getFront().pipelineBarrier(
      sourceStage,
      destinationStage,
      vk::DependencyFlagBits::eByRegion, // TODO: might be cause of an error
      0,
      nullptr,
      0,
      nullptr,
      1,
      &barrier
    );

    commandBuffer.end();

    m_info.layout = layout;
  }

  vk::Format Image::findSupportedFormat(vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& formatsToTest, vk::FormatFeatureFlagBits features, vk::ImageTiling tiling)
  {
    for (vk::Format format : formatsToTest)
    {
      auto props = physicalDevice.getFormatProperties(format);

      if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        return format;

      else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        return format;
    }

    RX_ERROR("Failed to retrieve any supported image format.");

    return vk::Format::eUndefined;
  }
}