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
      vk::DescriptorBufferInfo bufferInfo( uniformBuffers[i],               // buffer
                                           0,                               // offset
                                           sizeof( UniformBufferObject ) ); // range

      vk::DescriptorImageInfo imageInfo( textureSampler,                            // sampler
                                         textureImageView,                          // imageView
                                         vk::ImageLayout::eShaderReadOnlyOptimal ); // imageLayout

      // TODO: a lot of this information is already stored in the bindings which are part of DescriptorSetLayout. Re-use this here.
      std::array<vk::WriteDescriptorSet, 2> descriptorWrites{ };
      descriptorWrites[0] = writeUniformBuffer( m_sets[i], 0, bufferInfo );
      descriptorWrites[1] = writeCombinedImageSampler( m_sets[i], 1, imageInfo );

      g_device.updateDescriptorSets(descriptorWrites, 0);    
    }
  }

  void DescriptorSet::update( const std::vector<vk::Buffer>& uniformBuffers, vk::AccelerationStructureKHR tlas, vk::ImageView storageImageView )
  {
    for ( size_t i = 0; i < m_layouts.size( ); ++i )
    {
      vk::WriteDescriptorSetAccelerationStructureKHR descriptorInfoAS( 1,       // accelerationStructureCount
                                                                       &tlas ); // pAccelerationStructures 

      vk::DescriptorImageInfo imageInfo( { },                         // sampler
                                         storageImageView,            // imageView
                                         vk::ImageLayout::eGeneral ); // imageLayout

      vk::DescriptorBufferInfo bufferInfo( uniformBuffers[i],               // buffer
                                           0,                               // offset
                                           sizeof( UniformBufferObject ) ); // range

      std::array<vk::WriteDescriptorSet, 3> descriptorWrites;
      descriptorWrites[0] = writeAccelerationStructure( m_sets[i], 0, &descriptorInfoAS );
      descriptorWrites[1] = writeStorageImage( m_sets[i], 1, imageInfo );
      descriptorWrites[2] = writeUniformBuffer( m_sets[i], 2, bufferInfo );

      g_device.updateDescriptorSets( descriptorWrites, 0 );
    }
  }

  void DescriptorSet::free( )
  {
    g_device.freeDescriptorSets( m_descriptorPool, m_sets );
  }

  vk::WriteDescriptorSet DescriptorSet::writeUniformBuffer( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorBufferInfo& bufferInfo )
  {
    vk::WriteDescriptorSet result( descriptorSet,                      // dstSet
                                   binding,                            // dstBinding
                                   0,                                  // dstArrayElement
                                   1,                                  // descriptorCount
                                   vk::DescriptorType::eUniformBuffer, // descriptorType
                                   nullptr,                            // pImageInfo
                                   &bufferInfo,                        // pBufferInfo
                                   nullptr );                          // pTextelBufferView

    return result;
  }

  vk::WriteDescriptorSet DescriptorSet::writeStorageImage( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorImageInfo& imageInfo )
  {
    vk::WriteDescriptorSet result( descriptorSet,                     // dstSet
                                   binding,                           // dstBinding
                                   0,                                 // dstArrayElement
                                   1,                                 // descriptorCount
                                   vk::DescriptorType::eStorageImage, // descriptorType
                                   &imageInfo,                        // pImageInfo
                                   nullptr,                           // pBufferInfo
                                   nullptr );                         // pTextelBufferView

    return result;
  }

  vk::WriteDescriptorSet DescriptorSet::writeCombinedImageSampler( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorImageInfo& imageInfo )
  {
    vk::WriteDescriptorSet result( descriptorSet,                             // dstSet
                                   binding,                                   // dstBinding
                                   0,                                         // dstArrayElement
                                   1,                                         // descriptorCount
                                   vk::DescriptorType::eCombinedImageSampler, // descriptorType
                                   &imageInfo,                                // pImageInfo
                                   nullptr,                                   // pBufferInfo
                                   nullptr );                                 // pTextelBufferView

    return result;
  }

  vk::WriteDescriptorSet DescriptorSet::writeAccelerationStructure( vk::DescriptorSet descriptorSet, uint32_t binding, void* pNext )
  {
    vk::WriteDescriptorSet result( descriptorSet,                                 // dstSet
                                   binding,                                       // dstBinding
                                   0,                                             // dstArrayElement
                                   1,                                             // descriptorCount
                                   vk::DescriptorType::eAccelerationStructureKHR, // descriptorType
                                   nullptr,                                       // pImageInfo
                                   nullptr,                                       // pBufferInfo
                                   nullptr );                                     // pTextelBufferView
    result.pNext = pNext;

    return result;
  }
}