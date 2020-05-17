#ifndef COMMAND_BUFFERS_HPP
#define COMMAND_BUFFERS_HPP

#include "Swapchain.hpp"
#include "RenderPass.hpp"
#include "Pipeline.hpp"

namespace RX
{
  class CommandBuffers
  {
  public:
    inline std::vector<VkCommandBuffer>& get() { return m_commandBuffers; }

    void initialize(VkDevice device, VkCommandPool commandPool, size_t swapchainFramebufferSize);
    void record(Swapchain& spwachain, RenderPass& renderPass, Pipeline& pipeline);

  private:
    std::vector<VkCommandBuffer> m_commandBuffers;
  };
}

#endif // COMMAND_BUFFER_HPP