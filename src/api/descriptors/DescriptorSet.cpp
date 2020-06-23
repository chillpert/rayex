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
    allocInfo.descriptorSetCount = m_info.setCount;
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

      // TODO: a lot of this information is already stored in the bindings which are part of DescriptorSetLayout. Re-use this here.
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
    vk::WriteDescriptorSetAccelerationStructureKHR descriptorInfoAS;
    descriptorInfoAS.pNext = nullptr;
    descriptorInfoAS.accelerationStructureCount = 1;
    descriptorInfoAS.pAccelerationStructures = &info.tlas;

    vk::DescriptorImageInfo storageImageInfo;
    storageImageInfo.imageView = info.storageImageView;
    storageImageInfo.imageLayout = vk::ImageLayout::eGeneral;

    // TODO: a lot of this information is already stored in the bindings which are part of DescriptorSetLayout. Re-use this here.
    std::array<vk::WriteDescriptorSet, 2> descriptorWrites;
    descriptorWrites[0].pNext = &descriptorInfoAS;
    descriptorWrites[0].dstSet = m_sets[0];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].descriptorType = vk::DescriptorType::eAccelerationStructureKHR;

    descriptorWrites[1].dstSet = m_sets[0];
    descriptorWrites[1].descriptorType = vk::DescriptorType::eStorageImage;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].pImageInfo = &storageImageInfo;
    descriptorWrites[1].descriptorCount = 1;

    m_info.device.updateDescriptorSets(descriptorWrites, 0);
  }

  void DescriptorSet::destroy()
  {
    m_info.device.freeDescriptorSets(m_info.pool, m_sets);
  }
}