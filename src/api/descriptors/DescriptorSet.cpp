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
    
    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_info.pool;
    allocInfo.descriptorSetCount = m_info.layouts.size();
    allocInfo.pSetLayouts = m_info.layouts.data();

    m_sets = m_info.device.allocateDescriptorSets(allocInfo);

    for (const auto& set : m_sets)
    {
      if (!set)
        RX_ERROR("Failed to allocate descriptor sets.");
    }
  }

  void DescriptorSet::update(SwapchainUpdateDescriptorSetInfo& info)
  {
    for (size_t i = 0; i < m_info.layouts.size(); ++i)
    {
      vk::DescriptorBufferInfo bufferInfo;
      bufferInfo.buffer = info.uniformBuffers[i];
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      vk::DescriptorImageInfo imageInfo;
      imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
      imageInfo.imageView = info.textureImageView;
      imageInfo.sampler = info.textureSampler;

      std::array<vk::WriteDescriptorSet, 2> descriptorWrites{ };
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

  void DescriptorSet::update(UpdateRaytracingDescriptorSetInfo& info)
  {
    for (size_t i = 0; i < m_info.layouts.size(); ++i)
    {
      vk::WriteDescriptorSetAccelerationStructureKHR descriptorAS;
      descriptorAS.accelerationStructureCount = 1;
      descriptorAS.pAccelerationStructures = &info.tlas;

      vk::DescriptorImageInfo imageInfo;
      imageInfo.sampler = nullptr;
      imageInfo.imageView = info.storageImageView;
      imageInfo.imageLayout = vk::ImageLayout::eGeneral;

      std::array<vk::WriteDescriptorSet, 2> descriptorWrites{ };
      descriptorWrites[0].pNext = &descriptorAS;
      descriptorWrites[0].dstSet = m_sets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].descriptorType = vk::DescriptorType::eAccelerationStructureKHR;
      descriptorWrites[0].pImageInfo = nullptr;
      descriptorWrites[0].pBufferInfo = nullptr;
      descriptorWrites[0].pTexelBufferView = nullptr;

      descriptorWrites[1].pNext = &descriptorAS;
      descriptorWrites[1].dstSet = m_sets[i];
      descriptorWrites[1].dstBinding = 1;
      descriptorWrites[1].dstArrayElement = 0;
      descriptorWrites[1].descriptorCount = 1;
      descriptorWrites[1].descriptorType = vk::DescriptorType::eStorageImage;
      descriptorWrites[1].pImageInfo = &imageInfo;
      descriptorWrites[1].pBufferInfo = nullptr;
      descriptorWrites[1].pTexelBufferView = nullptr;

      vk::DescriptorBufferInfo vertexBufferDescriptor;
      vertexBufferDescriptor.buffer;
      vertexBufferDescriptor.offset = 0;
      vertexBufferDescriptor.range = VK_WHOLE_SIZE;

      vk::DescriptorBufferInfo indexBufferDescriptor;
      indexBufferDescriptor.buffer;
      indexBufferDescriptor.offset = 0;
      indexBufferDescriptor.range = VK_WHOLE_SIZE;

      m_info.device.updateDescriptorSets(descriptorWrites, 0);
    }
  }

  void DescriptorSet::destroy()
  {
    m_info.device.freeDescriptorSets(m_info.pool, m_sets);
  }
}