#include "api/Descriptor.hpp"
#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"
#include "api/misc/Components.hpp"

#define TRY_CAST( cast ) try { cast } catch ( const std::bad_any_cast& e ) { RX_ERROR( e.what( ) ); }

namespace RAYEXEC_NAMESPACE
{
  Descriptors::~Descriptors( )
  {
    destroy( );
  }

  void Descriptors::setBinding( const Binding& binding )
  {
    this->bindings[binding.binding] = vk::Helper::getDescriptorSetLayoutBinding( binding.binding, binding.descriptorType, binding.shaderStage );
    this->sizes[binding.binding] = binding.size;
  }

  void Descriptors::init( const std::vector<Binding>& descriptorBindings )
  {
    this->bindings.resize( descriptorBindings.size( ) );
    this->sizes.resize( descriptorBindings.size( ) );

    for ( const auto& binding : descriptorBindings )
    {
      this->bindings[binding.binding] = vk::Helper::getDescriptorSetLayoutBinding( binding.binding, binding.descriptorType, binding.shaderStage );
      this->sizes[binding.binding] = binding.size;
    }

    // Init descriptor set layout.
    this->layout = vk::Initializer::initDescriptorSetLayout( this->bindings );

    // One layout per swapchain image.
    this->layouts = { g_swapchainImageCount, this->layout};

    // Init descriptor pool.
    this->pool = vk::Initializer::initDescriptorPool( vk::Helper::getPoolSizes( this->bindings ), static_cast<uint32_t>( g_maxGeometryNodes ) * g_swapchainImageCount );

    // Init descriptor sets.
    this->sets = vk::Initializer::initDescriptorSets( this->pool, this->layout );
    this->externalSets = nullptr;
  }

  void Descriptors::init( const std::vector<Binding>& descriptorBindings, std::vector<vk::DescriptorSet>& externalDescriptorSets )
  {
    init( descriptorBindings );    
    this->externalSets = &externalDescriptorSets;
  }

  void Descriptors::update( const std::vector<std::any>& descriptors )
  {
    size_t a = this->bindings.size( ), b = this->sizes.size( ), c = descriptors.size( );
    RX_ASSERT( ( a == b && a == c && b == c ), "Failed to update descriptors because of missing create information." );

    std::vector<vk::WriteDescriptorSet> descriptorWrites;
    descriptorWrites.reserve( this->bindings.size( ) );

    for ( size_t i = 0; i < this->layouts.size( ); ++i )
    {
      for ( size_t j = 0; j < this->bindings.size( ); ++j )
      {
        switch ( this->bindings[j].descriptorType )
        {
          case vk::DescriptorType::eUniformBuffer:
          {
            std::vector<vk::Buffer> temp = std::any_cast<std::vector<vk::Buffer>>( descriptors[j] );
            descriptorWrites.push_back( writeBuffer( { temp[i], 0, this->sizes[j] }, i, j ) );
            break;
          }
          case vk::DescriptorType::eStorageBuffer:
          {
            vk::Buffer temp = std::any_cast<vk::Buffer>( descriptors[j] );
            descriptorWrites.push_back( writeBuffer( { temp, 0, this->sizes[j] }, i, j ) );
            break;
          }
          case vk::DescriptorType::eAccelerationStructureKHR:
          {
            vk::AccelerationStructureKHR temp = std::any_cast<vk::AccelerationStructureKHR>( descriptors[j] );
            descriptorWrites.push_back( writeAccelerationStructure( { 1, &temp }, i, j ) );
            break;
          }
          case vk::DescriptorType::eStorageImage:
          {
            vk::ImageView temp = std::any_cast<vk::ImageView>( descriptors[j] );
            descriptorWrites.push_back( writeImage( { { }, temp, vk::ImageLayout::eGeneral }, i, j ) );
            break;
          }
          case vk::DescriptorType::eCombinedImageSampler:
          {
            CombinedImageSampler temp = std::any_cast<CombinedImageSampler>( descriptors[j] );
            descriptorWrites.push_back( writeImage( { temp.sampler, temp.imageView, vk::ImageLayout::eGeneral }, i, j ) );
            break;
          }
          default:
          {
            RX_ERROR( "Descriptor not handled." );
            break;
          }
        }
      }

      g_device.updateDescriptorSets( descriptorWrites, 0 );
    }
  }

  vk::DescriptorSet Descriptors::determineSet( size_t setIndex, size_t bindingIndex )
  {
    vk::DescriptorSet temp;
    if ( this->externalSets == nullptr )
    {
      RX_ASSERT( setIndex < this->sets.size( ), "Descriptor sets out of range" );
      temp = this->sets[setIndex];
    }
    else
    {
      RX_ASSERT( setIndex < this->externalSets->size( ), "Descriptor sets out of range" );
      temp = this->externalSets->at( setIndex );
    }

    return temp;
  }

  vk::WriteDescriptorSet Descriptors::writeBuffer( vk::DescriptorBufferInfo info, size_t setIndex, size_t bindingIndex )
  {
    return vk::WriteDescriptorSet( determineSet( setIndex, bindingIndex ),      // dstSet
                                   this->bindings[bindingIndex].binding,        // dstBinding
                                   0,                                           // dstArrayElement
                                   1,                                           // descriptorCount
                                   this->bindings[bindingIndex].descriptorType, // descriptorType
                                   nullptr,                                     // pImageInfo
                                   &info,                                       // pBufferInfo
                                   nullptr );                                   // pTextelBufferView
  }

  vk::WriteDescriptorSet Descriptors::writeImage( vk::DescriptorImageInfo info, size_t setIndex, size_t bindingIndex )
  {
    return vk::WriteDescriptorSet( determineSet( setIndex, bindingIndex ),      // dstSet
                                   this->bindings[bindingIndex].binding,        // dstBinding
                                   0,                                           // dstArrayElement
                                   1,                                           // descriptorCount
                                   this->bindings[bindingIndex].descriptorType, // descriptorType
                                   &info,                                       // pImageInfo
                                   nullptr,                                     // pBufferInfo
                                   nullptr );                                   // pTextelBufferView
  }

  vk::WriteDescriptorSet Descriptors::writeAccelerationStructure( vk::WriteDescriptorSetAccelerationStructureKHR info, size_t setIndex, size_t bindingIndex )
  {
    vk::WriteDescriptorSet result( determineSet( setIndex, bindingIndex ),      // dstSet
                                   this->bindings[bindingIndex].binding,        // dstBinding
                                   0,                                           // dstArrayElement
                                   1,                                           // descriptorCount
                                   this->bindings[bindingIndex].descriptorType, // descriptorType
                                   nullptr,                                     // pImageInfo
                                   nullptr,                                     // pBufferInfo
                                   nullptr );                                   // pTextelBufferView

    result.pNext = &info;
    return result;
  }

  void Descriptors::destroy( )
  {
    if ( this->layout )
    {
      g_device.destroyDescriptorSetLayout( this->layout );
      this->layout = nullptr;
    }

    if ( this->pool )
    {
      g_device.destroyDescriptorPool( this->pool );
      this->pool = nullptr;
    }

    this->bindings.clear( );
    this->counter = 0;
  }
}