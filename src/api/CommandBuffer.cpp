#include "api/CommandBuffer.hpp"
#include "api/Device.hpp"

namespace RX
{
  CommandBuffer::CommandBuffer(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice, SwapChain* swapChain, Pipeline* pipeline) :
    m_commandPool(VK_NULL_HANDLE), m_physicalDevice(physicalDevice), m_logicalDevice(logicalDevice), m_swapChain(swapChain), m_pipeline(pipeline) { }

  void CommandBuffer::createCommandPool()
  {
    QueueFamilyIndices queueFamilyIndices = Device::findQueueFamilies(*m_physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    Assert::vulkan(vkCreateCommandPool(*m_logicalDevice, &poolInfo, nullptr, &m_commandPool), "Failed to create command pool");
  }

  void CommandBuffer::createCommandBuffers()
  {
    // allocate command buffers
    m_commandBuffers.resize(m_swapChain->getFramebuffers().size());

    VkCommandBufferAllocateInfo allocInfo { };
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    Assert::vulkan(vkAllocateCommandBuffers(*m_logicalDevice, &allocInfo, m_commandBuffers.data()), "Failed to allocate command buffers");

    startCommandBufferRecording();
  }

  void CommandBuffer::startCommandBufferRecording()
  {
    if (m_pipeline == nullptr)
    {
      Error::runtime("Can not start command buffer recording because the pipeline has not been set up yet", Error::PIPELINE);
    }

    if (m_swapChain == nullptr)
    {
      Error::runtime("Can not start command buffer recording because the swapchain has not been set up yet", Error::SWAPCHAIN);
    }

    // start command buffer recording
    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
      VkCommandBufferBeginInfo beginInfo { };
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      Assert::vulkan(vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo), "Failed to begin recording command buffer");
      
      VkRenderPassBeginInfo renderPassInfo { };
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = *m_pipeline->getRenderPass();
      renderPassInfo.framebuffer = m_swapChain->getFramebuffers()[i];
      renderPassInfo.renderArea.offset = { 0, 0 };
      renderPassInfo.renderArea.extent = m_swapChain->getExtent();

      VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
      renderPassInfo.clearValueCount = 1;
      renderPassInfo.pClearValues = &clearColor;

      vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
      vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline->getPipeline());
      vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);
      vkCmdEndRenderPass(m_commandBuffers[i]);
      
      Assert::vulkan(vkEndCommandBuffer(m_commandBuffers[i]), "Failed to record command buffer");
    }
  }

  void CommandBuffer::destroyCommandPool()
  {
    vkDestroyCommandPool(*m_logicalDevice, m_commandPool, nullptr);
  }
}