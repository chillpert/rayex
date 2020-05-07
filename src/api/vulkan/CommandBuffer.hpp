#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class CommandBuffer
  {
  public:
    CommandBuffer(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice);

    void createCommandPool();
    void destroyCommandPool();

  private:
    VkCommandPool m_commandPool;

    VkPhysicalDevice* m_physicalDevice;
    VkDevice* m_logicalDevice;
  };
}

#endif // COMMAND_BUFFER_HPP