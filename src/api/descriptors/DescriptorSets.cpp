#include "DescriptorSets.hpp"

namespace RX
{
  void DescriptorSets::initialize(VkDevice device, size_t swapchainImagesCount, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<Buffer>& uniformBuffers, Texture& texture)
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

      VkDescriptorImageInfo imageInfo{ };
      imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfo.imageView = texture.getImageView();
      imageInfo.sampler = texture.getSampler();

      std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = m_sets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[1].dstSet = m_sets[i];
      descriptorWrites[1].dstBinding = 1;
      descriptorWrites[1].dstArrayElement = 0;
      descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptorWrites[1].descriptorCount = 1;
      descriptorWrites[1].pImageInfo = &imageInfo;

      vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
  }
}