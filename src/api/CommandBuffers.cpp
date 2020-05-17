#include "CommandBuffers.hpp"

namespace RX
{
	void CommandBuffers::initialize(VkDevice device, VkCommandPool commandPool, size_t swapchainFramebufferSize)
	{
	  m_commandBuffers.resize(swapchainFramebufferSize);

	  VkCommandBufferAllocateInfo allocateInfo{ };
	  allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	  allocateInfo.commandPool = commandPool;
	  allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	  allocateInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	  VK_ASSERT(vkAllocateCommandBuffers(device, &allocateInfo, m_commandBuffers.data()), "Failed to allocate command buffers");
	}

	void CommandBuffers::record(const Swapchain& swapchain, const RenderPass& renderPass)
	{
		for (size_t i = 0; i < commandBuffers.size(); i++)
		{
	    VkCommandBufferBeginInfo beginInfo{ };
	    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	    VK_ASSERT(vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo), "Failed to record command buffers");

	    VkRenderPassBeginInfo renderPassInfo{ };
	    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	    renderPassInfo.renderPass = renderPass.get();
	    renderPassInfo.framebuffer = swapchain.getFramebuffers()[i];
	    renderPassInfo.renderArea.offset = { 0, 0 };
	    renderPassInfo.renderArea.extent = swapchain.getExtent();

	    VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	    renderPassInfo.clearValueCount = 1;
	    renderPassInfo.pClearValues = &clearColor;

	    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
      vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

	    vkCmdEndRenderPass(commandBuffers[i]);

	    VK_ASSERT(vkEndCommandBuffer(commandBuffers[i]), "Failed to record command buffers");
    }
	}
}