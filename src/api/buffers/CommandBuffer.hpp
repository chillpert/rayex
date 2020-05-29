#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct CommandBufferInfo
  {
    VkDevice device;
    VkCommandPool commandPool;
    VkQueue queue;
    size_t commandBufferCount = 1; // Amount of command buffers that will be created.
    VkCommandBufferUsageFlags usageFlags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    bool freeAutomatically = true;
    const char* componentName = "command buffer(s)"; // Optional, used to give a more precise name for the object which will improve logging output.

    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr, 0, nullptr }; // Ignore
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO, nullptr, 0, nullptr, nullptr, 0, nullptr, 0, nullptr}; // Ignore
  };

  class CommandBuffer
  {
  public:
    ~CommandBuffer();

    inline std::vector<VkCommandBuffer>& get() { return m_commandBuffers; }
    inline VkCommandBuffer getFront() { return m_commandBuffers[0]; }
    inline CommandBufferInfo& getInfo() { return m_info; }

    void initialize(CommandBufferInfo& info);
    void free();

    void begin(size_t index = 0);
    void end(size_t index = 0);
    void record();

  private:
    std::vector<VkCommandBuffer> m_commandBuffers;
    CommandBufferInfo m_info;

    bool m_allocated = false;
  };
}

#endif // COMMAND_BUFFER_HPP