#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class Memory
  {
  public:
    Memory() = default;
    Memory(vk::Image image, bool allocate = true);

    void alloc(vk::Image image);

    static uint32_t findType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);

  private:
    vk::UniqueDeviceMemory m_memory;
  };
}

#endif // MEMORY_HPP
