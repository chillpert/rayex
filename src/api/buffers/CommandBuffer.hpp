#ifndef COMMAND_BUFFER_HPP
#define COMMAND_BUFFER_HPP

#include "InitializerInfos.hpp"

namespace RX
{
  struct CmdBufferInfo
  {
    vk::Device device;
    vk::CommandPool commandPool;
    vk::Queue queue;

    size_t commandBufferCount = 1; // Amount of command buffers that will be created.
    vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary;
    vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    vk::CommandBufferResetFlags resetFlags = vk::CommandBufferResetFlagBits::eReleaseResources;
  };

  class CmdBuffer
  {
  public:
    CmdBuffer() = default;
    CmdBuffer(CmdBufferInfo& info); 
    CmdBuffer(CmdBufferInfo&& info);

    inline std::vector<vk::CommandBuffer>& get() { return m_commandBuffers; }
    inline vk::CommandBuffer get(size_t index) { return m_commandBuffers[index]; }
    inline vk::CommandBuffer getFront() { return m_commandBuffers[0]; }

    void init(CmdBufferInfo& info);
    void init(CmdBufferInfo&& info);

    void free();
    // Can only be used explicitly if the command pool's used to create the command buffer(s) 
    // from was created with resetFlags equal to VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT.
    void reset();

    void begin(size_t index = 0);
    void end(size_t index = 0);

    void submitToQueue(const vk::Queue queue) const;

    void setSubmitInfo(const vk::SubmitInfo& submitInfo);

  private:
    std::vector<vk::CommandBuffer> m_commandBuffers;

    CmdBufferInfo m_info;
    vk::CommandBufferBeginInfo m_beginInfo;
    vk::SubmitInfo m_submitInfo;

    bool m_allocated = false;
  };
}

#endif // COMMAND_BUFFER_HPP