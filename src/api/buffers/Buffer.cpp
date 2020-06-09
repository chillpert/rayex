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
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.size = m_info.deviceSize;
    bufferInfo.usage = m_info.usage;
    bufferInfo.sharingMode = m_info.sharingMode;

    if (m_info.sharingMode == vk::SharingMode::eConcurrent)
    {
      if (m_info.queueFamilyIndices.size() == 0)
      {
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;
      }
      else
      {
        RX_ASSERT((m_info.queueFamilyIndices.size() > 0), "Queue family indices were not specified for buffer creation");

        bufferInfo.pQueueFamilyIndices = m_info.queueFamilyIndices.data();
        bufferInfo.queueFamilyIndexCount = m_info.queueFamilyIndexCount;
      }
    }
    
    m_buffer = m_info.device.createBuffer(bufferInfo);
    if (!m_buffer)
      RX_ERROR("Failed to create " + m_info.componentName);

    // Allocate memory for the buffer.
    auto memRequirements = m_info.device.getBufferMemoryRequirements(m_buffer);

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.pNext = m_info.pNextMemory;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(m_info.physicalDevice, memRequirements.memoryTypeBits, createInfo.properties); 
    
    /*
    TODO:
       The right way to allocate memory for a large number of objects at the same time is to create a custom 
       allocator that splits up a single allocation among many different objects by using the offset parameters 
       that we've seen in many functions.
    */

    m_memory = m_info.device.allocateMemory(allocInfo);
    if (!m_memory)
      RX_ERROR("Failed to allocate memory for " + m_info.componentName);

    // Bind the buffer to the allocated memory.
    m_info.device.bindBufferMemory(m_buffer, m_memory, 0); // TODO: expose 0
  }
  
  void Buffer::destroy()
  {
    m_info.device.destroyBuffer(m_buffer);
    m_buffer = nullptr;

    m_info.device.freeMemory(m_memory);
    m_memory = nullptr;
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
    commandBufferInfo.componentName = "command buffer for copying a buffer to another buffer";
    
    CommandBuffer commandBuffer;
    commandBuffer.initialize(commandBufferInfo);

    commandBuffer.begin();

    vk::BufferCopy copyRegion;
    copyRegion.size = m_info.deviceSize;

    commandBuffer.getFront().copyBuffer(m_buffer, buffer.get(), 1, &copyRegion); // CMD

    commandBuffer.end();
  }

  void Buffer::copyToImage(Image& image) const
  {
    CommandBufferInfo commandBufferInfo{ };
    commandBufferInfo.device = image.getInfo().device;
    commandBufferInfo.commandPool = image.getInfo().commandPool;
    commandBufferInfo.queue = image.getInfo().queue;
    // TODO: assert that this queue has transfer capbalitites
    commandBufferInfo.freeAutomatically = true;
    commandBufferInfo.componentName = "command buffer for copying a buffer to an image";

    CommandBuffer commandBuffer;
    commandBuffer.initialize(commandBufferInfo);

    commandBuffer.begin();

    vk::BufferImageCopy region;
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = vk::Offset3D{ 0, 0, 0 };
    region.imageExtent = image.getInfo().extent;

    commandBuffer.getFront().copyBufferToImage(m_buffer, image.get(), vk::ImageLayout::eTransferDstOptimal, 1, &region); // CMD

    commandBuffer.end();
  }

  uint32_t Buffer::findMemoryType(vk::PhysicalDevice physicalDevice, uint32_t types, vk::MemoryPropertyFlags properties)
  {
    static vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
      if (types & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
    }

    RX_ERROR("Failed to find suitable memory type");
    return uint32_t();
  }
}