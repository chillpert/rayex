#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Memory // TODO: this should take care of vk::DeviceMemory and provide a custom allocator.
  {
  public:
    static uint32_t findType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties)
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
  };
}

#endif // MEMORY_HPP
