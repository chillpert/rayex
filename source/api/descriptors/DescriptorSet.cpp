#include "api/descriptors/DescriptorSet.hpp"
#include "api/misc/Components.hpp"

namespace RAYEXEC_NAMESPACE
{
  DescriptorSet::DescriptorSet( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts, bool initialize )
  {
    if ( initialize )
      init( descriptorPool, count, layouts );
  }

  void DescriptorSet::init( vk::DescriptorPool descriptorPool, uint32_t count, const std::vector<vk::DescriptorSetLayout>& layouts )
  {
    this->descriptorPool = descriptorPool;
    this->layouts = layouts;

    vk::DescriptorSetAllocateInfo allocInfo( descriptorPool,
                                             count,
                                             layouts.data() );

    this->sets = g_device.allocateDescriptorSets(allocInfo);

    for ( const vk::DescriptorSet& set : this->sets )
      RX_ASSERT( set, "Failed to create descriptor sets." );
  }

  void DescriptorSet::update( const std::vector<std::any>& data )
  {
    for ( size_t i = 0; i < this->layouts.size( ); ++i )
    {
      std::vector<vk::WriteDescriptorSet> descriptorWrites( data.size( ) );

      std::vector<std::pair<vk::WriteDescriptorSetAccelerationStructureKHR, size_t>> accelerationStructureInfos;
      std::vector<std::pair<vk::DescriptorImageInfo, size_t>> storageImageInfos;
      std::vector<std::pair<vk::DescriptorBufferInfo, size_t>> uniformInfos;
      std::vector<std::pair<vk::DescriptorImageInfo, size_t>> combinedImageSamplerInfo;
      std::vector<std::pair<vk::DescriptorBufferInfo, size_t>> bufferInfos;

      for ( size_t j = 0; j < data.size( ); ++j )
      {
        if ( typeid( AsDesc ) == data[j].type( ) )
        {
          AsDesc temp = std::any_cast<AsDesc>( data[j] );
          accelerationStructureInfos.push_back( { { 1, &temp.accelerationStructure }, j } );
        }
        else if ( typeid( StorageImageDesc ) == data[j].type( ) )
        {
          StorageImageDesc temp = std::any_cast<StorageImageDesc>( data[j] );
          storageImageInfos.push_back( { { { }, temp.imageView, vk::ImageLayout::eGeneral }, j } );
        }
        else if ( typeid( UboDesc ) == data[j].type( ) )
        {
          UboDesc temp = std::any_cast<UboDesc>( data[j] );
          uniformInfos.push_back( { { temp.uniformBuffers[i], 0, temp.size }, j } );
        }
        else if ( typeid( CombinedImageSamplerDesc ) == data[j].type( ) )
        {
          CombinedImageSamplerDesc temp = std::any_cast<CombinedImageSamplerDesc>( data[j] );
          combinedImageSamplerInfo.push_back( { { temp.sampler, temp.imageView, vk::ImageLayout::eShaderReadOnlyOptimal }, j } );
        }
        else if ( typeid( StorageBufferDesc ) == data[j].type( ) )
        {
          StorageBufferDesc temp = std::any_cast<StorageBufferDesc>( data[j] );
          bufferInfos.push_back( { { temp.storageBuffer, 0, temp.size }, j } );
        }
        else
        {
          RX_ERROR( "Descriptor can not be updated because its type is unvalid or not supported." );
        }
      }

      for ( const auto& it : accelerationStructureInfos )
        descriptorWrites[it.second] = writeAccelerationStructure( this->sets[i], it.second, &it.first );

      for ( const auto& it : storageImageInfos )
        descriptorWrites[it.second] = writeStorageImage( this->sets[i], it.second, it.first );

      for ( const auto& it : uniformInfos )
        descriptorWrites[it.second] = writeUniformBuffer( this->sets[i], it.second, it.first );

      for ( const auto& it : combinedImageSamplerInfo )
        descriptorWrites[it.second] = writeCombinedImageSampler( this->sets[i], it.second, it.first );
      
      for ( const auto& it : bufferInfos )
        descriptorWrites[it.second] = writeStorageBuffer( this->sets[i], it.second, it.first );

      g_device.updateDescriptorSets( descriptorWrites, 0 );
    }
  }
 
  void DescriptorSet::free( )
  {
    g_device.freeDescriptorSets( this->descriptorPool, this->sets );
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

  vk::WriteDescriptorSet DescriptorSet::writeAccelerationStructure( vk::DescriptorSet descriptorSet, uint32_t binding, const void* pNext )
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