#include "Texture.hpp"
#include "api/buffers/Buffer.hpp"
#include "api/buffers/CommandBuffer.hpp"

namespace RX
{
  Image::Image() :
    BaseComponent("Image") { }

  Image::~Image()
  {
    destroy();
  }
  
  void Image::initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImageCreateInfo& createInfo)
  {
    m_physicalDevice = physicalDevice;
    m_device = device;
    m_queue = queue;
    m_commandPool = commandPool;
    m_width = createInfo.extent.width;
    m_height = createInfo.extent.height;
    m_format = createInfo.format;
    m_layout = createInfo.initialLayout;

    VK_ASSERT(vkCreateImage(device, &createInfo, nullptr, &m_image), "Failed to create image");

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(device, m_image, &memoryRequirements);

    VkMemoryAllocateInfo allocateInfo{ };
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = Buffer::findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_ASSERT(vkAllocateMemory(device, &allocateInfo, nullptr, &m_memory), "Failed to allocate memory for image");

    vkBindImageMemory(device, m_image, m_memory, 0);

    RX_INITIALIZATION_CALLBACK;
  }

  void Image::transitionToLayout(VkImageLayout layout)
  {
    CommandBuffer commandBuffer;
    commandBuffer.begin(m_device, m_commandPool, m_queue);

    VkImageMemoryBarrier barrier{ };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = m_layout;
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

    if (m_layout == VK_IMAGE_LAYOUT_UNDEFINED && layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (m_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
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
      commandBuffer.get(),
      sourceStage, destinationStage,
      0,
      0, nullptr,
      0, nullptr,
      1, &barrier
    );

    commandBuffer.end();

    m_layout = layout;
  }

  void Image::destroy()
  {
    RX_ASSERT_DESTRUCTION;

    vkDestroyImage(m_device, m_image, nullptr);
    vkFreeMemory(m_device, m_memory, nullptr);
  }
}