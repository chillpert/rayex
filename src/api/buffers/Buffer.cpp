#include "Buffer.hpp"
#include "CommandBuffer.hpp"

namespace RX
{
  Buffer::~Buffer()
  {
    destroy();
  }

  void Buffer::initialize(BufferCreateInfo& createInfo)
  {
    m_info = createInfo;

    // Create the buffer.
    VkBufferCreateInfo bufferInfo{ };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = createInfo.deviceSize;
    bufferInfo.usage = createInfo.usage;
    bufferInfo.sharingMode = createInfo.sharingMode;
    
    VK_CREATE(vkCreateBuffer(createInfo.device, &bufferInfo, nullptr, &m_buffer), "vertex buffer");

    // Allocate memory for the buffer.
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(createInfo.device, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{ };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(m_info.physicalDevice, memRequirements.memoryTypeBits, createInfo.properties);

    /*
    TODO:
       The right way to allocate memory for a large number of objects at the same time is to create a custom 
       allocator that splits up a single allocation among many different objects by using the offset parameters 
       that we've seen in many functions.
    */
    VK_ASSERT(vkAllocateMemory(createInfo.device, &allocInfo, nullptr, &m_memory), "Failed to allocate memory for buffer");

    // Bind the buffer to the allocated memory.
    VK_ASSERT(vkBindBufferMemory(createInfo.device, m_buffer, m_memory, 0), "Failed to bind buffer to memory");
  }
  
  void Buffer::destroy()
  {
    VK_DESTROY(vkDestroyBuffer(m_info.device, m_buffer, nullptr), "buffer");
    VK_FREE(vkFreeMemory(m_info.device, m_memory, nullptr), "buffer");

    m_buffer = VK_NULL_HANDLE;
    m_memory = VK_NULL_HANDLE;
  }

  Buffer& Buffer::operator=(const Buffer& buffer)
  {
    buffer.copyToBuffer(*this);
    return *this;
  }

  void Buffer::copyToBuffer(const Buffer& buffer) const
  {
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = m_info.device;
    commandBufferInfo.commandPool = m_info.commandPool;
    commandBufferInfo.queue = m_info.queue;
    commandBufferInfo.freeAutomatically = true;

    CommandBuffer commandBuffer;
    commandBuffer.initialize(commandBufferInfo);

    commandBuffer.begin();

    VkBufferCopy copyRegion{};
    copyRegion.size = m_info.deviceSize;

    vkCmdCopyBuffer(commandBuffer.getFront(), m_buffer, buffer.get(), 1, &copyRegion);

    commandBuffer.end();
  }

  void Buffer::copyToImage(Image& image) const
  {
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = image.getInfo().device;
    commandBufferInfo.commandPool = image.getInfo().commandPool;
    commandBufferInfo.queue = image.getInfo().queue;

    CommandBuffer commandBuffer;
    commandBuffer.initialize(commandBufferInfo);

    commandBuffer.begin();

    VkBufferImageCopy region{ };
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = image.getInfo().extent;

    vkCmdCopyBufferToImage(commandBuffer.getFront(), m_buffer, image.get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    commandBuffer.end();
  }

  uint32_t Buffer::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
  {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
      if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
    }

    RX_ERROR("Failed to find suitable memory type");
    return uint32_t();
  }
}