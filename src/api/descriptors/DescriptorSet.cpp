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

  void DescriptorSet::update( vk::AccelerationStructureKHR tlas, vk::ImageView storageImageView, const std::vector<vk::Buffer>& uniformBuffers )
  {
    for ( size_t i = 0; i < m_layouts.size( ); ++i )
    {
      vk::WriteDescriptorSetAccelerationStructureKHR descriptorInfoAS( 1,       // accelerationStructureCount
                                                                       &tlas ); // pAccelerationStructures 

      vk::DescriptorImageInfo imageInfo( { },                         // sampler
                                         storageImageView,            // imageView
                                         vk::ImageLayout::eGeneral ); // imageLayout

      vk::DescriptorBufferInfo cameraBufferInfo( uniformBuffers[i],      // buffer
                                                 0,                      // offset
                                                 sizeof( CameraUbo ) );  // range

      std::array<vk::WriteDescriptorSet, 3> descriptorWrites;
      descriptorWrites[0] = writeAccelerationStructure( m_sets[i], 0, &descriptorInfoAS );
      descriptorWrites[1] = writeStorageImage( m_sets[i], 1, imageInfo );
      descriptorWrites[2] = writeUniformBuffer( m_sets[i], 2, cameraBufferInfo );

      g_device.updateDescriptorSets( descriptorWrites, 0 );
    }
  }

  void DescriptorSet::update( vk::ImageView textureImageView, vk::Sampler textureSampler )
  {
    for ( size_t i = 0; i < m_layouts.size( ); ++i )
    {
      vk::DescriptorImageInfo textureInfo( textureSampler,                            // sampler
                                           textureImageView,                          // imageView
                                           vk::ImageLayout::eShaderReadOnlyOptimal ); // imageLayout

      g_device.updateDescriptorSets( writeCombinedImageSampler( m_sets[i], 0, textureInfo ), 0 );
    }
  }

  void DescriptorSet::update( vk::Buffer vertexBuffer, vk::Buffer indexBuffer )
  {
    for ( size_t i = 0; i < m_layouts.size( ); ++i )
    {
      vk::DescriptorBufferInfo vertbufferInfo( vertexBuffer,         // buffer
                                           0,                        // offset
                                           sizeof( vertexBuffer ) ); // range

      vk::DescriptorBufferInfo indexbufferInfo( indexBuffer,         // buffer
                                           0,                        // offset
                                           sizeof( indexBuffer ) );  // range

      std::array<vk::WriteDescriptorSet, 2> descriptorWrites;
      descriptorWrites[0] = writeStorageBuffer( m_sets[i], 0, vertbufferInfo );
      descriptorWrites[1] = writeStorageBuffer( m_sets[i], 1, indexbufferInfo );

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

  vk::WriteDescriptorSet DescriptorSet::writeStorageBuffer( vk::DescriptorSet descriptorSet, uint32_t binding, const vk::DescriptorBufferInfo& bufferInfo )
  {
    vk::WriteDescriptorSet result( descriptorSet,                      // dstSet
                                   binding,                            // dstBinding
                                   0,                                  // dstArrayElement
                                   1,                                  // descriptorCount
                                   vk::DescriptorType::eStorageBuffer, // descriptorType
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