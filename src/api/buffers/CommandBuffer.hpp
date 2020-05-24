#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  // A Commandbuffer for single usage.
  class CommandBuffer
  {
  public:
    inline VkCommandBuffer get() { return m_commandBuffer; }

    void begin(VkDevice device, VkCommandPool commandPool, VkQueue queue);
    void end();

  private:
    VkCommandBuffer m_commandBuffer;

    VkDevice m_device;
    VkCommandPool m_commandPool;
    VkQueue m_queue;
  };
}

#endif // COMMAND_BUFFER_HPP