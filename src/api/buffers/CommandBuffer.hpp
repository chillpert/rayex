#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  struct CommandBufferInfo
  {
    vk::Device device;
    vk::CommandPool commandPool;
    vk::Queue queue;

    size_t commandBufferCount = 1; // Amount of command buffers that will be created.
    vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    vk::CommandBufferResetFlags resetFlags = vk::CommandBufferResetFlagBits::eReleaseResources;
    vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary;
    bool freeAutomatically = true;
    bool submitAutomatically = true; // Only submits automatically if usageFlags contains ONE_TIME_SUBMIT
    const char* componentName = "command buffer(s)"; // Optional, used to give a more precise name for the object which will improve logging output.

    vk::CommandBufferBeginInfo beginInfo; // Ignore
    vk::SubmitInfo submitInfo; // Ignore
  };

  class CommandBuffer
  {
  public:
    inline std::vector<vk::CommandBuffer>& get() { return m_commandBuffers; } // TODO: getter where you can an index.
    inline vk::CommandBuffer getFront() { return m_commandBuffers[0]; }
    inline CommandBufferInfo& getInfo() { return m_info; }

    void initialize(CommandBufferInfo& info);
    void free();
    // Can only be used explicitly if the command pool's used to create the command buffer(s) 
    // from was created with resetFlags equal to VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT.
    void reset();

    void begin(size_t index = 0);
    void end(size_t index = 0);

  private:
    std::vector<vk::CommandBuffer> m_commandBuffers;
    CommandBufferInfo m_info;

    bool m_allocated = false;
  };
}

#endif // COMMAND_BUFFER_HPP