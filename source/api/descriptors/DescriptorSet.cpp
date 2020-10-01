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
      std::vector<std::pair<vk::DescriptorImageInfo, size_t>> combinedImageSamplerInfo;
      std::vector<std::pair<vk::DescriptorBufferInfo, size_t>> uniformInfos;
      std::vector<std::pair<vk::DescriptorBufferInfo, size_t>> storageBufferInfos;
      std::vector<std::pair<vk::DescriptorBufferInfo, size_t>> dynamicBufferInfos;
      std::vector<std::tuple<std::vector<vk::DescriptorBufferInfo>, size_t, uint32_t>> storageBufferArrayInfos;

      // I do not know why. But for some reason I can not set the descriptor writes in one single loop. This is the weirdest bug I have ever seen.
      for ( size_t j = 0; j < data.size( ); ++j )
      {
        if ( typeid( AccelerationStructureDescriptor ) == data[j].type( ) )
        {
          AccelerationStructureDescriptor temp = std::any_cast<AccelerationStructureDescriptor>( data[j] );
          accelerationStructureInfos.push_back( { { 1, &temp.accelerationStructure }, j } );
        }
        else if ( typeid( StorageImageDescriptor ) == data[j].type( ) )
        {
          StorageImageDescriptor temp = std::any_cast<StorageImageDescriptor>( data[j] );
          storageImageInfos.push_back( { { { }, temp.imageView, vk::ImageLayout::eGeneral }, j } );
        }
        else if ( typeid( UboDescriptor ) == data[j].type( ) )
        {
          UboDescriptor temp = std::any_cast<UboDescriptor>( data[j] );
          uniformInfos.push_back( { { temp.uniformBuffers[i], 0, temp.size }, j } );
        }
        else if ( typeid( CombinedImageSamplerDescriptor ) == data[j].type( ) )
        {
          CombinedImageSamplerDescriptor temp = std::any_cast<CombinedImageSamplerDescriptor>( data[j] );
          combinedImageSamplerInfo.push_back( { { temp.sampler, temp.imageView, vk::ImageLayout::eShaderReadOnlyOptimal }, j } );
        }
        else if ( typeid( StorageBufferDescriptor ) == data[j].type( ) )
        {
          StorageBufferDescriptor temp = std::any_cast<StorageBufferDescriptor>( data[j] );
          storageBufferInfos.push_back( { { temp.storageBuffer, 0, temp.size }, j } );
        }
        else if ( typeid( DynamicStorageBufferDescriptor ) == data[j].type( ) )
        {
          DynamicStorageBufferDescriptor temp = std::any_cast<DynamicStorageBufferDescriptor>( data[j] );
          dynamicBufferInfos.push_back( { { temp.storageBuffer, 0, temp.size }, j } );
        }
        else if ( typeid( StorageBufferArrayDescriptor ) == data[j].type( ) )
        {
          StorageBufferArrayDescriptor temp = std::any_cast<StorageBufferArrayDescriptor>( data[j] );

          std::vector<vk::DescriptorBufferInfo> bufferInfos;
          bufferInfos.reserve( temp.storageBuffers.size( ) );

          for ( const auto& storageBuffer : temp.storageBuffers )
          {
            bufferInfos.push_back( { storageBuffer, 0, temp.size } );
          }

          storageBufferArrayInfos.push_back( { bufferInfos, j, static_cast<uint32_t>( temp.storageBuffers.size( ) ) } );
        }
        else
        {
          RX_ERROR( "Descriptor can not be updated because its type is unvalid or not supported." );
        }
      }

      for ( const auto& it : accelerationStructureInfos )
      {
        vk::WriteDescriptorSet write( this->sets[i],                                 // dstSet
                                      it.second,                                     // dstBinding
                                      0,                                             // dstArrayElement
                                      1,                                             // descriptorCount
                                      vk::DescriptorType::eAccelerationStructureKHR, // descriptorType
                                      nullptr,                                       // pImageInfo
                                      nullptr,                                       // pBufferInfo
                                      nullptr );                                     // pTextelBufferView
        write.pNext = &it.first;

        descriptorWrites[it.second] = write;
      }

      for ( const auto& it : storageImageInfos )
      {
        vk::WriteDescriptorSet write( this->sets[i],                     // dstSet
                                      it.second,                         // dstBinding
                                      0,                                 // dstArrayElement
                                      1,                                 // descriptorCount
                                      vk::DescriptorType::eStorageImage, // descriptorType
                                      &it.first,                         // pImageInfo
                                      nullptr,                           // pBufferInfo
                                      nullptr );                         // pTextelBufferView

        descriptorWrites[it.second] = write;
      }

      for ( const auto& it : uniformInfos )
      {
        vk::WriteDescriptorSet write( this->sets[i],                      // dstSet
                                      it.second,                          // dstBinding
                                      0,                                  // dstArrayElement
                                      1,                                  // descriptorCount
                                      vk::DescriptorType::eUniformBuffer, // descriptorType
                                      nullptr,                            // pImageInfo
                                      &it.first,                          // pBufferInfo
                                      nullptr );                          // pTextelBufferView

        descriptorWrites[it.second] = write;
      }

      for ( const auto& it : combinedImageSamplerInfo )
      {
        vk::WriteDescriptorSet write( this->sets[i],                             // dstSet
                                      it.second,                                 // dstBinding
                                      0,                                         // dstArrayElement
                                      1,                                         // descriptorCount
                                      vk::DescriptorType::eCombinedImageSampler, // descriptorType
                                      &it.first,                                 // pImageInfo
                                      nullptr,                                   // pBufferInfo
                                      nullptr );                                 // pTextelBufferView
      
        descriptorWrites[it.second] = write;
      }
      
      for ( const auto& it : storageBufferInfos )
      {
        vk::WriteDescriptorSet write( this->sets[i],                      // dstSet
                                      it.second,                          // dstBinding
                                      0,                                  // dstArrayElement
                                      1,                                  // descriptorCount
                                      vk::DescriptorType::eStorageBuffer, // descriptorType
                                      nullptr,                            // pImageInfo
                                      &it.first,                          // pBufferInfo
                                      nullptr );                          // pTextelBufferView

        descriptorWrites[it.second] = write;
      }

      for ( const auto& it : dynamicBufferInfos )
      {
        vk::WriteDescriptorSet write( this->sets[i],                             // dstSet
                                      it.second,                                 // dstBinding
                                      0,                                         // dstArrayElement
                                      1,                                         // descriptorCount
                                      vk::DescriptorType::eStorageBufferDynamic, // descriptorType
                                      nullptr,                                   // pImageInfo
                                      &it.first,                                 // pBufferInfo
                                      nullptr );                                 // pTextelBufferView

        descriptorWrites[it.second] = write;
      }

      for ( const auto& it : storageBufferArrayInfos )
      {
        vk::WriteDescriptorSet write( this->sets[i],                      // dstSet
                                      std::get<1>( it ),                  // dstBinding
                                      0,                                  // dstArrayElement
                                      std::get<2>( it ),                  // descriptorCount
                                      vk::DescriptorType::eStorageBuffer, // descriptorType
                                      nullptr,                            // pImageInfo
                                      std::get<0>( it ).data( ),          // pBufferInfo
                                      nullptr );                          // pTextelBufferView

        descriptorWrites[std::get<1>( it )] = write;
      }

      g_device.updateDescriptorSets( descriptorWrites, 0 );
    }
  }

  void DescriptorSet::update( const std::vector<vk::WriteDescriptorSet> writes )
  {
    g_device.updateDescriptorSets( static_cast<uint32_t>( writes.size( ) ), writes.data( ), 0, nullptr );
  }
 
  void DescriptorSet::free( )
  {
    g_device.freeDescriptorSets( this->descriptorPool, this->sets );
  }
}