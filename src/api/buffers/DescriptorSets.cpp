#include "DescriptorSets.hpp"

namespace RX
{
  void DescriptorSets::initialize(VkDevice device, size_t swapchainImagesCount, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<Buffer>& uniformBuffers)
  {
    std::vector<VkDescriptorSetLayout> layouts(swapchainImagesCount, descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{ };
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapchainImagesCount);
    allocInfo.pSetLayouts = layouts.data();

    m_sets.resize(swapchainImagesCount);

    VK_ASSERT(vkAllocateDescriptorSets(device, &allocInfo, m_sets.data()), "Failed to allocate descriptor sets.");

    for (size_t i = 0; i < swapchainImagesCount; ++i)
    {
      VkDescriptorBufferInfo bufferInfo{ };
      bufferInfo.buffer = uniformBuffers[i].get();
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      VkWriteDescriptorSet writeSet{ };
      writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writeSet.dstSet = m_sets[i];
      writeSet.dstBinding = 0;
      writeSet.dstArrayElement = 0;
      writeSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      writeSet.descriptorCount = 1;
      writeSet.pBufferInfo = &bufferInfo;

      vkUpdateDescriptorSets(device, 1, &writeSet, 0, nullptr);
    }
  }
}