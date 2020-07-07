#include "DescriptorSet.hpp"
#include "Components.hpp"

namespace rx
{
  DescriptorSet::DescriptorSet( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts, bool initialize )
  {
    if ( initialize )
      init( descriptorPool, count, layouts );
  }

  void DescriptorSet::init( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts )
  {
    m_descriptorPool = descriptorPool;
    m_layouts = layouts;

    vk::DescriptorSetAllocateInfo allocInfo( descriptorPool,
                                             count,
                                             layouts.data() );

    m_sets = g_device.allocateDescriptorSets(allocInfo);

    for ( const vk::DescriptorSet& set : m_sets )
      RX_ASSERT( set, "Failed to create descriptor sets." );
  }

  void DescriptorSet::update( const std::vector<vk::Buffer>& uniformBuffers, vk::ImageView textureImageView, vk::Sampler textureSampler )
  {
    for (size_t i = 0; i < m_layouts.size(); ++i)
    {
      vk::DescriptorBufferInfo bufferInfo;
      bufferInfo.buffer = uniformBuffers[i];
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      vk::DescriptorImageInfo imageInfo;
      imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
      imageInfo.imageView = textureImageView;
      imageInfo.sampler = textureSampler;

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

      g_device.updateDescriptorSets(descriptorWrites, 0);    
    }
  }

  void DescriptorSet::update( vk::AccelerationStructureKHR tlas, vk::ImageView storageImageView )
  {
    vk::WriteDescriptorSetAccelerationStructureKHR descriptorInfoAS( { }, // accelerationStructureCount
                                                                     { } ); // pAccelerationStructures 
    
  }

  void DescriptorSet::free( )
  {
    g_device.freeDescriptorSets( m_descriptorPool, m_sets );
  }
}