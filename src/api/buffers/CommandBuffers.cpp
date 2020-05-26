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

	void CommandBuffers::record(Swapchain& swapchain, Framebuffers& framebuffers, RenderPass& renderPass, Pipeline& pipeline, std::vector<std::shared_ptr<Model>>& models)
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

			VkClearValue colorValue;
			colorValue.color =	{0.0f, 0.0f, 0.0f, 1.0f};
			VkClearValue depthValue;
			depthValue.depthStencil = {1.0f, 0};

			VkClearValue clearValues[] = { colorValue, depthValue };
	    renderPassInfo.clearValueCount = 2;
	    renderPassInfo.pClearValues = clearValues;

	    vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());

				for (std::shared_ptr<Model> model : models)
				{
					VkBuffer vertexBuffers[] = { model->m_vertexBuffer.get() };
					VkDeviceSize offsets[] = { 0 };
					vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

					vkCmdBindIndexBuffer(m_commandBuffers[i], model->m_indexBuffer.get(), 0, model->m_indexBuffer.getType());

					vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 0, 1, &model->m_descriptorSets.get()[i], 0, nullptr);

					vkCmdDrawIndexed(m_commandBuffers[i], model->m_indexBuffer.getCount(), 1, 0, 0, 0);
				}

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