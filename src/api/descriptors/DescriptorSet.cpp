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
    
    std::vector<vk::DescriptorSetLayout> layouts(m_info.swapchainImagesCount, m_info.descriptorSetLayout);

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_info.descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(m_info.swapchainImagesCount);
    allocInfo.pSetLayouts = layouts.data();

    m_sets = m_info.device.allocateDescriptorSets(allocInfo);

    for (size_t i = 0; i < m_info.swapchainImagesCount; ++i)
    {
      vk::DescriptorBufferInfo bufferInfo;
      bufferInfo.buffer = m_info.uniformBuffers[i];
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      vk::DescriptorImageInfo imageInfo;
      imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
      imageInfo.imageView = m_info.textureImageView;
      imageInfo.sampler = m_info.textureSampler;

      std::array<vk::WriteDescriptorSet, 2> descriptorWrites{};
      descriptorWrites[0].dstSet = m_sets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      descriptorWrites[1].dstSet = m_sets[i];
      descriptorWrites[1].dstBinding = 1;
      descriptorWrites[1].dstArrayElement = 0;
      descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
      descriptorWrites[1].descriptorCount = 1;
      descriptorWrites[1].pImageInfo = &imageInfo;

      m_info.device.updateDescriptorSets(descriptorWrites, 0);
    }
  }

  void DescriptorSet::destroy()
  {
    m_info.device.freeDescriptorSets(m_info.descriptorPool, m_sets);
  }
}