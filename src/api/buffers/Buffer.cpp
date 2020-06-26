#include "Buffer.hpp"
#include "CommandBuffer.hpp"
#include "Memory.hpp"

namespace RX
{
  Buffer::Buffer(BufferInfo& createInfo)
  {
    init(createInfo);
  }

  Buffer::Buffer(BufferInfo&& createInfo)
  {
    init(createInfo);
  }

  Buffer::~Buffer()
  {
    if (m_buffer && m_memory)
      destroy();
  }

  void Buffer::init(BufferInfo& createInfo)
  {
    m_info = createInfo;

    // Create the buffer.
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.pNext = m_info.pNextBuffer;
    bufferInfo.flags = m_info.bufferFlags;
    bufferInfo.size = m_info.size;
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

        bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(m_info.queueFamilyIndices.size());
        bufferInfo.pQueueFamilyIndices = m_info.queueFamilyIndices.data();
      }
    }
    
    m_buffer = m_info.device.createBuffer(bufferInfo);
    if (!m_buffer)
      RX_ERROR("Failed to create buffer");

    // Allocate memory for the buffer.
    auto memRequirements = m_info.device.getBufferMemoryRequirements(m_buffer);

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.pNext = m_info.pNextMemory;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Memory::findType(m_info.physicalDevice, memRequirements.memoryTypeBits, m_info.memoryProperties);
    
    /*
    TODO:
       The right way to allocate memory for a large number of objects at the same time is to create a custom 
       allocator that splits up a single allocation among many different objects by using the offset parameters 
       that we've seen in many functions.
    */

    m_memory = m_info.device.allocateMemory(allocInfo);
    if (!m_memory)
      RX_ERROR("Failed to allocate memory for buffer.");

    // Bind the buffer to the allocated memory.
    m_info.device.bindBufferMemory(m_buffer, m_memory, m_info.memoryOffset);
  }

  void Buffer::init(BufferInfo&& info)
  {
    init(info);
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
    CmdBuffer commandBuffer({ m_info.device, m_info.commandPool });
    commandBuffer.begin();
    {
      vk::BufferCopy copyRegion;
      copyRegion.size = m_info.size;

      commandBuffer.getFront().copyBuffer(m_buffer, buffer.get(), 1, &copyRegion); // CMD
    }
    commandBuffer.end();
    commandBuffer.submitToQueue(m_info.queue);
  }

  void Buffer::copyToImage(Image& image) const
  {
    // TODO: assert that this queue has transfer capbalitites
    CmdBuffer commandBuffer({ m_info.device, m_info.commandPool });
    commandBuffer.begin();
    {
      vk::BufferImageCopy region;
      region.bufferOffset = 0;
      region.bufferRowLength = 0;
      region.bufferImageHeight = 0;
      region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
      region.imageSubresource.mipLevel = 0;
      region.imageSubresource.baseArrayLayer = 0;
      region.imageSubresource.layerCount = 1;
      region.imageOffset = vk::Offset3D{ 0, 0, 0 };
      region.imageExtent = image.getExtent();

      commandBuffer.getFront().copyBufferToImage(m_buffer, image.get(), vk::ImageLayout::eTransferDstOptimal, 1, &region); // CMD
    }
    commandBuffer.end();
    commandBuffer.submitToQueue(m_info.queue);
  }
}