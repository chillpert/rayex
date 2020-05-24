#include "CommandBuffers.hpp"
#include "Vertex.hpp"

namespace RX
{
	CommandBuffers::CommandBuffers() :
		BaseComponent("CommandBuffers") { }

	CommandBuffers::~CommandBuffers()
	{
		destroy();
	}

	void CommandBuffers::initialize(VkDevice device, VkCommandPool commandPool, size_t swapchainFramebufferSize)
	{
		m_device = device;
		m_commandPool = commandPool;

	  m_commandBuffers.resize(swapchainFramebufferSize);

	  VkCommandBufferAllocateInfo allocateInfo{ };
	  allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	  allocateInfo.commandPool = commandPool;
	  allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	  allocateInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	  VK_ASSERT(vkAllocateCommandBuffers(device, &allocateInfo, m_commandBuffers.data()), "Failed to allocate command buffers");
	
		RX_INITIALIZATION_CALLBACK;
	}

	void CommandBuffers::record(Swapchain& swapchain, Framebuffers& framebuffers, RenderPass& renderPass, Pipeline& pipeline, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, DescriptorSets& descriptorSets)
	{
		RX_ASSERT_INITIALIZED("record");

		for (size_t i = 0; i < m_commandBuffers.size(); ++i)
		{
	    VkCommandBufferBeginInfo beginInfo{ };
	    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	    VK_ASSERT(vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo), "Failed to record command buffers");

	    VkRenderPassBeginInfo renderPassInfo{ };
	    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	    renderPassInfo.renderPass = renderPass.get();
	    renderPassInfo.framebuffer = framebuffers.get()[i];
	    renderPassInfo.renderArea.offset = { 0, 0 };
	    renderPassInfo.renderArea.extent = swapchain.getExtent();

	    VkClearValue clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	    renderPassInfo.clearValueCount = 1;
	    renderPassInfo.pClearValues = &clearColor;

	    vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());

				VkBuffer vertexBuffers[] = { vertexBuffer.get() };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

				vkCmdBindIndexBuffer(m_commandBuffers[i], indexBuffer.get(), 0, indexBuffer.getType());

				vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 0, 1, &descriptorSets.get()[i], 0, nullptr);

				vkCmdDrawIndexed(m_commandBuffers[i], indexBuffer.getCount(), 1, 0, 0, 0);

	    vkCmdEndRenderPass(m_commandBuffers[i]);

	    VK_ASSERT(vkEndCommandBuffer(m_commandBuffers[i]), "Failed to record command buffers");
    }
	}

	void CommandBuffers::free()
	{
		RX_ASSERT_DESTRUCTION;
		vkFreeCommandBuffers(m_device, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
	}

	void CommandBuffers::destroy()
	{
		free();
	}
}