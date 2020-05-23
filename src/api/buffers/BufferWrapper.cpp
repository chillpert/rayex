#include "BufferWrapper.hpp"

namespace RX
{
  BufferWrapper::BufferWrapper() :
    BaseComponent("BufferWrapper") { }

  BufferWrapper::~BufferWrapper()
  {
    destroy();
  }

  void BufferWrapper::create(BufferCreateInfo& createInfo)
  {
    m_info = createInfo;

    // Create the buffer.
    VkBufferCreateInfo bufferInfo{ };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size =  m_info.size;
    bufferInfo.usage = m_info.usage;
    bufferInfo.sharingMode = m_info.sharingMode;

    VK_ASSERT(vkCreateBuffer(m_info.device, &bufferInfo, nullptr, &m_buffer), "Failed to create vertex buffer");

    // Allocate memory for the buffer.
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_info.device, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{ };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, m_info.properties);

    /*  
    TODO:
       The right way to allocate memory for a large number of objects at the same time is to create a custom 
       allocator that splits up a single allocation among many different objects by using the offset parameters 
       that we've seen in many functions.
    */
    VK_ASSERT(vkAllocateMemory(m_info.device, &allocInfo, nullptr, &m_memory), "Failed to allocate memory for buffer");

    // Bind the buffer to the allocated memory.
    VK_ASSERT(vkBindBufferMemory(m_info.device, m_buffer, m_memory, 0), "Failed to bind buffer to memory");
  
    initializationCallback();
  }
  
  void BufferWrapper::destroy()
  {
    assertDestruction();

    vkDestroyBuffer(m_info.device, m_buffer, nullptr);
    vkFreeMemory(m_info.device, m_memory, nullptr);
  }

  BufferWrapper& BufferWrapper::operator=(const BufferWrapper& buffer)
  {
    buffer.copyTo(*this);
    return *this;
  }

  void BufferWrapper::copyTo(const BufferWrapper& buffer) const
  {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_info.commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_info.device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = m_info.size;
    vkCmdCopyBuffer(commandBuffer, m_buffer, buffer.get(), 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_info.queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_info.queue);

    vkFreeCommandBuffers(m_info.device, m_info.commandPool, 1, &commandBuffer);
  }

  uint32_t BufferWrapper::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
  {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_info.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
      if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
    }

    RX_ERROR("Failed to find suitable memory type");
  }
}