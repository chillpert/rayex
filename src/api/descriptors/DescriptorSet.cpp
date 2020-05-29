#include "DescriptorSet.hpp"

namespace RX
{
  DescriptorSet::~DescriptorSet()
  {
    //destroy();
  }

  void DescriptorSet::initialize(DescriptorSetInfo& info)
  {
    m_info = info;
    
    std::vector<VkDescriptorSetLayout> layouts(m_info.swapchainImagesCount, m_info.descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{ };
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_info.descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(m_info.swapchainImagesCount);
    allocInfo.pSetLayouts = layouts.data();

    m_sets.resize(m_info.swapchainImagesCount);

    VK_ALLOCATE(vkAllocateDescriptorSets(m_info.device, &allocInfo, m_sets.data()), "descriptor sets");
    
    for (size_t i = 0; i < m_info.swapchainImagesCount; ++i)
    {
      VkDescriptorBufferInfo bufferInfo{ };
      bufferInfo.buffer = m_info.uniformBuffers[i];
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      VkDescriptorImageInfo imageInfo{ };
      imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfo.imageView = m_info.textureImageView;
      imageInfo.sampler = m_info.textureSampler;

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

      vkUpdateDescriptorSets(m_info.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
  }

  void DescriptorSet::destroy()
  {
    VK_FREE(vkFreeDescriptorSets(m_info.device, m_info.descriptorPool, static_cast<uint32_t>(m_sets.size()), m_sets.data()), "descriptor sets");
  }
}