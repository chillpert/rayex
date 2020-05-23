#ifndef COMMAND_BUFFERS_HPP
#define COMMAND_BUFFERS_HPP

#include "swapchain/Swapchain.hpp"
#include "swapchain/Framebuffers.hpp"
#include "RenderPass.hpp"
#include "Pipeline.hpp"
#include "buffers/Buffer.hpp"

namespace RX
{
  class CommandBuffers : public BaseComponent
  {
  public:
    CommandBuffers();

    inline std::vector<VkCommandBuffer>& get() { return m_commandBuffers; }

    void initialize(VkDevice device, VkCommandPool commandPool, size_t swapchainFramebufferSize);
    void record(Swapchain& spwachain, Framebuffers& framebuffers, RenderPass& renderPass, Pipeline& pipeline, Buffer& vertexBuffer, Buffer& indexBuffer);
    void free();

  private:
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkDevice m_device;
    VkCommandPool m_commandPool;
  };
}

#endif // COMMAND_BUFFER_HPP