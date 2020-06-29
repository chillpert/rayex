#include "Memory.hpp"
#include "Components.hpp"

namespace RX
{
  Memory::Memory(vk::Image image, bool allocate)
  {
    if (allocate)
      alloc(image);
  }

  void Memory::alloc(vk::Image image)
  {
    auto memoryRequirements = g_device.getImageMemoryRequirements(image);

    vk::MemoryAllocateInfo allocateInfo( memoryRequirements.size,                                                                                   // allocationSize 
                                         findType(g_physicalDevice, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal));  // memoryTypeIndex

    m_memory = g_device.allocateMemoryUnique(allocateInfo);
    if (!m_memory)
      RX_ERROR("Failed to create memory for image.");

    g_device.bindImageMemory(image, m_memory.get(), 0);
  }

  uint32_t Memory::findType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties)
  {
    static vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
      if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
    }

    RX_ERROR("Failed to find suitable memory type");
    return uint32_t();
  }
}