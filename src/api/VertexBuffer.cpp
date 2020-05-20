#include "VertexBuffer.hpp"

namespace RX
{
  VertexBuffer::VertexBuffer() :
    BaseComponent("VertexBuffer") { }

  VertexBuffer::~VertexBuffer()
  {
    destroy();
  }

  void VertexBuffer::initialize(VkDevice device, VkPhysicalDevice physicalDevice)
  {
    m_device = device;
    m_physicalDevice = physicalDevice;

    // Create the buffer.
    VkBufferCreateInfo bufferInfo{ };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_ASSERT(vkCreateBuffer(device, &bufferInfo, nullptr, &m_buffer), "Failed to create vertex buffer");

    // Allocate the Buffer.
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{ };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VK_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, &m_memory), "Failed to allocate memory for vertex buffer");

    // Bind buffer.
    VK_ASSERT(vkBindBufferMemory(device, m_buffer, m_memory, 0), "Failed to bind buffer to memory");

    // Fill the buffer.
    void* data;
    vkMapMemory(device, m_memory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferInfo.size);
    vkUnmapMemory(device, m_memory);

    initializationCallback();
  }

  void VertexBuffer::destroy()
  {
    assertDestruction();
    vkDestroyBuffer(m_device, m_buffer, nullptr);
    vkFreeMemory(m_device, m_memory, nullptr);
  }

  uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
}