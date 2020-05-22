#include "Buffer.hpp"

namespace RX
{
  Buffer::Buffer(const char* componentName) :
    BaseComponent(componentName) { }

  uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
  {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
      if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
    }

    RX_ERROR("Failed to find suitable memory type");
  }
  
  void Buffer::create(VkBufferUsageFlags usage, VkSharingMode sharingMode)
  {
    VkBufferCreateInfo bufferInfo{ };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size =  m_size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = sharingMode;

    VK_ASSERT(vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_buffer), "Failed to create vertex buffer");
  }

  void Buffer::allocate(VkMemoryPropertyFlags properties)
  {
    // Allocate the Buffer.
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{ };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    VK_ASSERT(vkAllocateMemory(m_device, &allocInfo, nullptr, &m_memory), "Failed to allocate memory for buffer");
  }

  void Buffer::bind()
  {
    VK_ASSERT(vkBindBufferMemory(m_device, m_buffer, m_memory, 0), "Failed to bind buffer to memory");
  }
}