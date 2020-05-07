#include "api/Vulkan/CommandBuffer.hpp"
#include "api/Vulkan/Device.hpp"

namespace RX
{
  CommandBuffer::CommandBuffer(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice, SwapChain* swapChain) :
    m_commandPool(VK_NULL_HANDLE), m_physicalDevice(physicalDevice), m_logicalDevice(logicalDevice), m_swapChain(swapChain) { }

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
    m_commandBuffers.resize(m_swapChain->getFramebuffers().size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    Assert::vulkan(vkAllocateCommandBuffers(*m_logicalDevice, &allocInfo, m_commandBuffers.data()), "Failed to allocate command buffers");
  }

  void CommandBuffer::destroyCommandPool()
  {
    vkDestroyCommandPool(*m_logicalDevice, m_commandPool, nullptr);
  }
}