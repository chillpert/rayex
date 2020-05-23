#ifndef COMMAND_BUFFERS_HPP
#define COMMAND_BUFFERS_HPP

#include "api/swapchain/Swapchain.hpp"
#include "api/swapchain/Framebuffers.hpp"
#include "api/RenderPass.hpp"
#include "api/Pipeline.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "DescriptorSets.hpp"

namespace RX
{
  // TODO: move to buffers directory
  class CommandBuffers : public BaseComponent
  {
  public:
    CommandBuffers();
    ~CommandBuffers();

    inline std::vector<VkCommandBuffer>& get() { return m_commandBuffers; }

    void initialize(VkDevice device, VkCommandPool commandPool, size_t swapchainFramebufferSize);
    void record(Swapchain& spwachain, Framebuffers& framebuffers, RenderPass& renderPass, Pipeline& pipeline, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, DescriptorSets& descriptorSets);
    void free();
    void destroy();

  private:
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkDevice m_device;
    VkCommandPool m_commandPool;
  };
}

#endif // COMMAND_BUFFER_HPP