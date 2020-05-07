#include "api/Vulkan/CommandBuffer.hpp"
#include "api/Vulkan/Device.hpp"

namespace RX
{
  CommandBuffer::CommandBuffer(VkPhysicalDevice* physicalDevice, VkDevice* logicalDevice) :
    m_commandPool(VK_NULL_HANDLE), m_physicalDevice(physicalDevice), m_logicalDevice(logicalDevice) { }

  void CommandBuffer::createCommandPool()
  {
    QueueFamilyIndices queueFamilyIndices = Device::findQueueFamilies(*m_physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    Assert::vulkan(vkCreateCommandPool(*m_logicalDevice, &poolInfo, nullptr, &m_commandPool), "Failed to create command pool");
  }

  void CommandBuffer::destroyCommandPool()
  {
    vkDestroyCommandPool(*m_logicalDevice, m_commandPool, nullptr);
  }
}